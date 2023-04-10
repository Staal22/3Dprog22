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

void VisualObject::computeVertexNormals()
{
    if (indexed)
    {
        int numFaces = mIndices.size() / 3;
        // Step 1: Compute face normals
        std::vector<QVector3D> faceNormals;
        for (int i = 0; i < numFaces; ++i)
        {
            const Vertex& v1 = mVertices[mIndices[i * 3]];
            const Vertex& v2 = mVertices[mIndices[i * 3 + 1]];
            const Vertex& v3 = mVertices[mIndices[i * 3 + 2]];
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

            for (int j = 0; j < numFaces; ++j)
            {
                bool sharesVertex = false;
                for (int k = 0; k < 3; ++k) {
                    if (mIndices[j * 3 + k] == i)
                    {
                        sharesVertex = true;
                        break;
                    }
                }
                if (!sharesVertex) continue;

                const Vertex& v1 = mVertices[mIndices[j * 3]];
                const Vertex& v2 = mVertices[mIndices[j * 3 + 1]];
                const Vertex& v3 = mVertices[mIndices[j * 3 + 2]];
                QVector3D faceNormal = faceNormals[j];
                QVector3D edge1 = v2.m_xyz - v1.m_xyz;
                QVector3D edge2 = v3.m_xyz - v1.m_xyz;
                float weight = qAcos(QVector3D::dotProduct(edge1.normalized(), edge2.normalized()));
                normalSum += faceNormal * weight;
                ++numFacesShared;
            }

            if (numFacesShared > 0) {
                mVertices[i].m_normal = normalSum / numFacesShared;
                mVertices[i].m_normal.normalize();
    //            qDebug() << "Normals changed" << mVertices[i].m_normal;
            }
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
