#include "objectloader.h"

ObjectLoader::ObjectLoader()
{

}

std::vector<Vertex> ObjectLoader::loadObjFile(const std::string &filename)
{
    std::vector<Vertex> vertices;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return vertices;
    }

    std::vector<QVector3D> positions;
    std::vector<QVector3D> normals;
    std::vector<QVector2D> textureCoords;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        if (token == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            positions.push_back(QVector3D(x, y, z));
        } else if (token == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            normals.push_back(QVector3D(x, y, z));
        } else if (token == "vt") {
            float u, v;
            iss >> u >> v;
            textureCoords.push_back(QVector2D(u, v));
        } else if (token == "f") {
            std::string vertexStr;
            std::vector<Vertex> faceVertices;
            while (iss >> vertexStr) {
                std::istringstream vss(vertexStr);
                int positionIdx, textureCoordIdx, normalIdx;
                vss >> positionIdx;
                if (textureCoords.size() > 0) {
                    vss.ignore(1);
                    vss >> textureCoordIdx;
                }
                if (normals.size() > 0) {
                    vss.ignore(1);
                    vss >> normalIdx;
                }
                Vertex v;
                v.m_xyz = positions[positionIdx - 1];
                if (textureCoords.size() > 0) {
                    v.m_uv = textureCoords[textureCoordIdx - 1];
                }
                if (normals.size() > 0) {
                    v.m_normal = normals[normalIdx - 1];
                }
                faceVertices.push_back(v);
            }
            // triangulate the face
            for (int i = 1; i < faceVertices.size() - 1; ++i) {
                vertices.push_back(faceVertices[0]);
                vertices.push_back(faceVertices[i]);
                vertices.push_back(faceVertices[i + 1]);
            }
        }
    }

    return vertices;
}
