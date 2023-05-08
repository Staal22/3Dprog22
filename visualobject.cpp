#include "visualobject.h"
#include "qimage.h"
#include "qopengltexture.h"
#include <cstddef>

VisualObject::VisualObject()
{
    model.setToIdentity();
}

VisualObject::~VisualObject()
{
    // Disable the vertex attribute arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}

void VisualObject::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    if (indexed)
    {
        glGenBuffers(1, &mIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);
    }

    //send vertex data to the GPU
    glBufferData( GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_xyz)));

    // 2nd attribute buffer : normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_normal)));

    if (hasTexture || hasHeightMap)
    {
        // 3rd attribute buffer : texture coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_uv)));

        QOpenGLTexture* texture;

        QImage image;
        image.load(texturepath);
        texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        texture->setData(image);
        texture->setWrapMode(QOpenGLTexture::Repeat);
        texture->setMinificationFilter(QOpenGLTexture::Nearest);
        texture->setMagnificationFilter(QOpenGLTexture::Nearest);
        if (!hasHeightMap)
            texture->bind(0);
        else
            texture->bind(1);
    }


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    if (hasTexture || hasHeightMap)
        glEnableVertexAttribArray(2);

    //    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
}

void VisualObject::draw(GLint shader)
{
    if (hide)
        return;
    modelUniform = shader;
    initializeOpenGLFunctions();
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, model.constData());

    if (indexed)
    {
        glDrawElements(drawMethod, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
    }
    else
    {
        glDrawArrays(drawMethod, 0, mVertices.size());
    }
}

std::pair<float, float> VisualObject::getPosition2D()
{
    auto col = model.column(3);
    return std::pair<float,float>(col.x(), col.y());
}

QVector3D VisualObject::getPosition3D()
{
    auto col = model.column(3);
    return QVector3D(col.x(), col.y(), col.z());
}

QMatrix4x4 VisualObject::getModelMatrix()
{
    return model;
}

struct QVector3DHash
{
    std::size_t operator()(const QVector3D& v) const
    {
        return qHash(v.x()) ^ qHash(v.y()) ^ qHash(v.z());
    }
};

struct QVector3DEqual
{
    bool operator()(const QVector3D& v1, const QVector3D& v2) const
    {
        return qFuzzyCompare(v1.x(), v2.x()) &&
               qFuzzyCompare(v1.y(), v2.y()) &&
               qFuzzyCompare(v1.z(), v2.z());
    }
};

void VisualObject::computeVertexNormals()
{
    if (indexed)
    {
        // Step 1: Compute face normals
        std::vector<QVector3D> faceNormals(mVertices.size(), QVector3D(0, 0, 0));
        for (int i = 0; i < mIndices.size(); i += 3)
        {
            const Vertex& v1 = mVertices[mIndices[i]];
            const Vertex& v2 = mVertices[mIndices[i + 1]];
            const Vertex& v3 = mVertices[mIndices[i + 2]];
            QVector3D edge1 = v2.m_xyz - v1.m_xyz;
            QVector3D edge2 = v3.m_xyz - v1.m_xyz;
            QVector3D normal = QVector3D::crossProduct(edge1, edge2);
            normal.normalize();
            faceNormals[mIndices[i]] += normal;
            faceNormals[mIndices[i + 1]] += normal;
            faceNormals[mIndices[i + 2]] += normal;
        }

        // Step 2: Compute vertex normals
        std::unordered_map<QVector3D, QVector3D, QVector3DHash, QVector3DEqual> vertexNormals;
        for (int i = 0; i < mVertices.size(); ++i)
        {
            const Vertex& v = mVertices[i];
            auto it = vertexNormals.find(v.m_xyz);
            if (it != vertexNormals.end()) {
                it->second += faceNormals[i];
            } else {
                vertexNormals[v.m_xyz] = faceNormals[i];
            }
        }

        for (auto& p : vertexNormals)
        {
            p.second.normalize();
        }

        for (auto& v : mVertices)
        {
            v.m_normal = vertexNormals[v.m_xyz];
        }
    }
    else
    {
        // Step 1: Compute face normals
        std::vector<QVector3D> faceNormals;
        for (int i = 0; i < mVertices.size(); i += 3)
        {
            const Vertex& v1 = mVertices[i];
            const Vertex& v2 = mVertices[i + 1];
            const Vertex& v3 = mVertices[i + 2];
            QVector3D edge1 = v2.m_xyz - v1.m_xyz;
            QVector3D edge2 = v3.m_xyz - v1.m_xyz;
            QVector3D normal = QVector3D::crossProduct(edge1, edge2);
            normal.normalize();
            faceNormals.push_back(normal);
        }

        // Step 2: Compute vertex normals
        for (int i = 0; i < mVertices.size(); ++i)
        {
            QVector3D normalSum(0, 0, 0);
            int numFacesShared = 0;

            for (int j = 0; j < faceNormals.size(); ++j)
            {
                const Vertex& v1 = mVertices[j * 3];
                const Vertex& v2 = mVertices[j * 3 + 1];
                const Vertex& v3 = mVertices[j * 3 + 2];

                if (v1.m_xyz == mVertices[i].m_xyz ||
                    v2.m_xyz == mVertices[i].m_xyz ||
                    v3.m_xyz == mVertices[i].m_xyz)
                {
                    normalSum += faceNormals[j];
                    ++numFacesShared;
                }
            }

            if (numFacesShared > 0) {
                mVertices[i].m_normal = normalSum / numFacesShared;
                mVertices[i].m_normal.normalize();
            }
        }
    }
}
