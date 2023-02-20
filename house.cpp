#include "house.h"

House::House()
{
    door1 = QVector3D(0, 0, 0.75f); // 9
    door2 = QVector3D(0, 0.6f, 0.75f); // 10
    door3 = QVector3D(0, 0, 1.25f); // 11
    door4 = QVector3D(0, 0.6f, 1.25f); // 12

    // Define the indices of the house shape
    std::vector<unsigned int> indices =
    {
        0, 1, 2, 2, 3, 0,    // roof
        4, 0, 3,
        4, 3, 2,
        4, 2, 1,
        4, 1, 0,
        0, 5, 16, // wall 1
        16, 13, 0,
        13, 15, 17,
        17, 14, 13,
        10, 9, 11, // door
        11, 12, 10,
        14, 18, 8, // cont. wall 1
        8, 3, 14,
        3, 8, 7, // wall 2
        7, 2, 3,
        2, 7, 6, // wall 3
        6, 1, 2,
        1, 6, 5, // wall 4
        5, 0, 1
    };

    for (long long unsigned int i = 0; i < indices.size(); ++i)
    {
        mIndices.push_back(indices[i]);
    }
}

House::~House()
{

}

void House::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    //

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), reinterpret_cast<const void*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<const void*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // Her kommer et tillegg som har med Index Buffer Object og indeksarray å gjøre
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    mRotation.setToIdentity();
    glBindVertexArray(0);

    mMatrix.scale(5);
//    mMatrix.translate(3, 0, 0);
//    mMatrix.rotate(-45.f, 0, 1, 0);

    // calculate the minimum and maximum points of the bounding box
    QVector3D door = (door1 + door3 ) / 2;
    min_ = QVector3D{door.x(), door.y(), door.z()} - QVector3D(0.5f / 2.f, 1.f / 2.f, 0.5f / 2.f);
    max_ = QVector3D{door.x(), door.y(), door.z()} + QVector3D(0.5f / 2.f, 1.f / 2.f, 0.5f / 2.f);
}

void House::draw()
{
//    mMatrix.rotate(1.f, 0, 1, 0);
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}

void House::open()
{
    mVertices.clear();

    door1 = QVector3D(0.5f, 0, 1.25f); // 9
    door2 = QVector3D(0.5f, 0.6f, 1.25f); // 10
    houseVertices();

    init(mMatrixUniform);
}

bool House::contains(QVector3D point) const
{
    return point.x() >= min_.x() && point.x() <= max_.x() &&
           point.y() >= min_.y() && point.y() <= max_.y() &&
           point.z() >= min_.z() && point.z() <= max_.z();
}

void House::houseVertices()
{
    // Define the vertices of the house shape
    QVector3D v1(0, 1, 0); // 0
    QVector3D v2(1, 1, 0); // 1
    QVector3D v3(1, 1, 2); // 2
    QVector3D v4(0, 1, 2); // 3
    QVector3D v5(0.5f, 2, 1); // 4
    QVector3D v6(0, 0, 0); // 5
    QVector3D v7(1, 0, 0); // 6
    QVector3D v8(1, 0, 2); // 7
    QVector3D v9(0, 0, 2); // 8
    QVector3D v14(0, 1, 0.75f); // 13
    QVector3D v15(0, 1, 1.25f); // 14
    QVector3D v16(0, 0.6f, 0.75f); // 15
    QVector3D v17(0, 0, 0.75f); // 16
    QVector3D v18(0, 0.6f, 1.25f); // 17
    QVector3D v19(0, 0, 1.25f); // 18

    QVector3D red(1, 0, 0);
    QVector3D green(0, 1, 0);

    // Add the vertices to the vector
    mVertices.push_back(Vertex{v1, red}); //0
    mVertices.push_back(Vertex{v2, red}); //1
    mVertices.push_back(Vertex{v3, red}); //2
    mVertices.push_back(Vertex{v4, red}); //3
    mVertices.push_back(Vertex{v5, red}); //4
    mVertices.push_back(Vertex{v6, red}); //5
    mVertices.push_back(Vertex{v7, red}); //6
    mVertices.push_back(Vertex{v8, red}); //7
    mVertices.push_back(Vertex{v9, red}); //8
    mVertices.push_back(Vertex{door1, green}); //9
    mVertices.push_back(Vertex{door2, green}); //10
    mVertices.push_back(Vertex{door3, green}); //11
    mVertices.push_back(Vertex{door4, green}); //12
    mVertices.push_back(Vertex{v14, red}); //13
    mVertices.push_back(Vertex{v15, red}); //14
    mVertices.push_back(Vertex{v16, red}); //15
    mVertices.push_back(Vertex{v17, red}); //16
    mVertices.push_back(Vertex{v18, red}); //17
    mVertices.push_back(Vertex{v19, red}); //18
}
