#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "Vertex.h"

#include <QVector3D>
#include <QVector2D>

class ObjectLoader
{
public:
    ObjectLoader();

//    struct Vertex {
//        QVector3D position;
//        QVector3D normal;
//        QVector2D textureCoord;
//    };

    std::vector<Vertex> loadObjFile(const std::string& filename);


};

#endif // OBJECTLOADER_H
