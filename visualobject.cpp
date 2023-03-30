#include "visualobject.h"

VisualObject::VisualObject()
{
    model.setToIdentity();
}

VisualObject::~VisualObject() {
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
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
    auto col = mPosition.column(3);
    return std::pair<float,float>(col.x(), col.y());
}

QVector3D VisualObject::getPosition3D()
{
    auto col = mPosition.column(3);
    return QVector3D(col.x(), col.y(), col.z());
}

void VisualObject::computeVertexNormals()
{
    if (!indexed)
        return;
    int numFaces = mIndices.size() / 3;
    // Step 1: Compute face normals
    std::vector<QVector3D> faceNormals;
    for (int i = 0; i < numFaces; ++i) {
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
    for (int i = 0; i < mVertices.size(); ++i) {
        QVector3D normalSum(0, 0, 0);
        int numFacesShared = 0;

        for (int j = 0; j < numFaces; ++j) {
            bool sharesVertex = false;
            for (int k = 0; k < 3; ++k) {
                if (mIndices[j * 3 + k] == i) {
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
