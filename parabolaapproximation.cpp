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
    model.setToIdentity();

    if (points)
        drawMethod = GL_POINTS;
    else
        drawMethod = GL_LINE_LOOP;
    indexed = false;
}

ParabolaApproximation::~ParabolaApproximation()
{

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
    model.setToIdentity();
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


