#include "parabolaapproximation.h"

ParabolaApproximation::ParabolaApproximation(bool inPoints)
{
    points = inPoints;
    mVertices.push_back(Vertex{1,7,0,0,1,0});
    mVertices.push_back(Vertex{3,2,0,0,1,0});
    mVertices.push_back(Vertex{6,1,0,0,1,0});
    mVertices.push_back(Vertex{7,2,0,0,1,0});
    mVertices.push_back(Vertex{8,4,0,0,1,0});
    mVertices.push_back(Vertex{9,5,0,0,1,0});
    mVertices.push_back(Vertex{10,7,0,0,1,0});
    mMatrix.setToIdentity();
}

ParabolaApproximation::~ParabolaApproximation()
{

}

void ParabolaApproximation::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    //VAO is a conntainer that holds VBOs
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    //since the mVAO is bound, this VBO will belong to that VAO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //send vertex data to the GPU
    glBufferData( GL_ARRAY_BUFFER,  //buffer type
                  mVertices.size()*sizeof( Vertex ),    //buffer size
                  mVertices.data(), //the vertices themselves
                  GL_STATIC_DRAW ); //whether we update the buffer on the GPU

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0,                                 //attribute - must match layout
                          3,                                 //size
                          GL_FLOAT,                          //data type
                          GL_FALSE,                          //normalized?
                          sizeof(Vertex),                    //stride
                          reinterpret_cast<GLvoid*>(0));     //array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    // Same paramater list as above, but attribute and offset are adjusted accordingly
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    //release vertex array bind(0) = release lol
    glBindVertexArray(0);
}

void ParabolaApproximation::draw()
{
    //what object to draw
    glBindVertexArray(mVAO);
    //Since our shader uses a matrix and we rotate the triangle, we send the current matrix here
    //Must be here to update each frame - if static object, it could be set only once
    glUniformMatrix4fv(mMatrixUniform,          //the location of the matrix in the shader
                       1,                       //count
                       GL_FALSE,                //transpose the matrix before sending it?
                       mMatrix.constData());    //the data of the matrix
    //DRAW CALL MOMENT
    if (points)
        glDrawArrays(GL_POINTS, 0, mVertices.size());
    else
        glDrawArrays(GL_LINE_LOOP, 0, mVertices.size());
}

void ParabolaApproximation::fit(const std::vector<Vertex> &points)
{
    int n = points.size();
    std::vector<double> x(n), y(n);
    for (int i = 0; i < n; i++)
    {
        x[i] = points[i].m_xyz[0];
        y[i] = points[i].m_xyz[1];
    }

    // Set up the matrix A and vector b for the linear system
    // A * x = b, where x are the coefficients of the polynomial
    std::vector<std::vector<double>> A(n, std::vector<double>(3));
    std::vector<double> bV(n);
    for (int i = 0; i < n; i++)
    {
        A[i][0] = x[i] * x[i];
        A[i][1] = x[i];
        A[i][2] = 1.0;
        bV[i] = y[i];
    }

    // Solve the linear system using the QR decomposition
    std::vector<double> x_ = solve(A, bV);

    a = x_[0];
    b = x_[1];
    c = x_[2];
}

void ParabolaApproximation::replace(double xMin, double xMax)
{
    mVertices.clear();
    double h = 0.05f;
    for (double x = xMin; x < xMax; x += h)
    {
        mVertices.push_back(Vertex{static_cast<float>(x),static_cast<float>(evaluate(x)),0,0,0,1});
    }
    mMatrix.setToIdentity();
}

// Evaluate the fitted parabola at a given x
double ParabolaApproximation::evaluate(double x) const
{
    return a * x * x + b * x + c;
}

std::vector<double> ParabolaApproximation::solve(const std::vector<std::vector<double>> &A, const std::vector<double> &b)
{
    int n = A.size();
    int m = A[0].size();

    // Perform QR decomposition
    std::vector<std::vector<double>> Q(n, std::vector<double>(m));
    std::vector<std::vector<double>> R(m, std::vector<double>(m));
    for (int j = 0; j < m; j++)
    {
        for (int i = 0; i < n; i++)
        {
            Q[i][j] = A[i][j];
        }
        for (int k = 0; k < j; k++)
        {
            double dot = 0.0;
            for (int i = 0; i < n; i++)
            {
                dot += Q[i][j] * Q[i][k];
            }
            for (int i = 0; i < n; i++)
            {
                Q[i][j] -= dot * Q[i][k];
            }
        }
        double norm = 0.0;
        for (int i = 0; i < n; i++)
        {
            norm += Q[i][j] * Q[i][j];
        }
        norm = sqrt(norm);
        for (int i = 0; i < n; i++)
        {
            Q[i][j] /= norm;
        }
        for (int i = 0; i < m; i++)
        {
            R[j][i] = 0.0;
            for (int k = 0; k < n; k++)
            {
                R[j][i] += Q[k][j] * A[k][i];
            }
        }
    }
    // Solve the system R * x = Q^T * b using backward substitution
    std::vector<double> x(m);
    for (int i = m - 1; i >= 0; i--)
    {
        x[i] = 0.0;
        for (int k = i + 1; k < m; k++)
        {
            x[i] -= R[i][k] * x[k];
        }
        x[i] += dotProduct(Q, b, i);
        x[i] /= R[i][i];
    }

    return x;
}

double ParabolaApproximation::dotProduct(const std::vector<std::vector<double>> &A, const std::vector<double> &b, int j)
{
    int n = A.size();
    double dot = 0.0;
    for (int i = 0; i < n; i++)
    {
        dot += A[i][j] * b[i];
    }
    return dot;
}


