#include <chrono>
#include <fstream>
#include <random>
#include <iostream>
using namespace std;

vector<vector<int>> bfmm(const vector<vector<int>> X, const vector<vector<int>> Y, int &count)
{
    int n = X.size();
    vector<vector<int>> Z(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
            {
                Z[i][j] += X[i][k] * Y[k][j];
                count++;
            }
    return Z;
}

vector<vector<int>> addMatrices(const vector<vector<int>> &A, const vector<vector<int>> &B)
{
    int n = A.size();

    vector<vector<int>> result(n, vector<int>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            result[i][j] = A[i][j] + B[i][j];
        }
    }

    return result;
}

vector<vector<int>> subtractMatrices(const vector<vector<int>> &A, const vector<vector<int>> &B)
{
    int n = A.size();
    vector<vector<int>> result(n, vector<int>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            result[i][j] = A[i][j] - B[i][j];
        }
    }

    return result;
}

vector<vector<int>> matrix_multiply(const vector<vector<int>> A, const vector<vector<int>> B, int &count)
{
    int n = A.size();
    vector<vector<int>> result(n, vector<int>(n));

    if (n == 1)
    {
        result[0][0] = A[0][0] * B[0][0];
        count++;
        return result;
    }

    int mid = n / 2;
    vector<vector<int>> a11(mid, vector<int>(mid));
    vector<vector<int>> a12(mid, vector<int>(mid));
    vector<vector<int>> a21(mid, vector<int>(mid));
    vector<vector<int>> a22(mid, vector<int>(mid));
    vector<vector<int>> b11(mid, vector<int>(mid));
    vector<vector<int>> b12(mid, vector<int>(mid));
    vector<vector<int>> b21(mid, vector<int>(mid));
    vector<vector<int>> b22(mid, vector<int>(mid));

    // Divide matrices into 4 submatrices
    for (int i = 0; i < mid; i++)
    {
        for (int j = 0; j < mid; j++)
        {
            a11[i][j] = A[i][j];
            a12[i][j] = A[i][j + mid];
            a21[i][j] = A[i + mid][j];
            a22[i][j] = A[i + mid][j + mid];

            b11[i][j] = B[i][j];
            b12[i][j] = B[i][j + mid];
            b21[i][j] = B[i + mid][j];
            b22[i][j] = B[i + mid][j + mid];
        }
    }

    // 7 products of submatrices
    vector<vector<int>> p1 = matrix_multiply(a11, subtractMatrices(b12, b22), count);
    vector<vector<int>> p2 = matrix_multiply(addMatrices(a11, a12), b22, count);
    vector<vector<int>> p3 = matrix_multiply(addMatrices(a21, a22), b11, count);
    vector<vector<int>> p4 = matrix_multiply(a22, subtractMatrices(b21, b11), count);
    vector<vector<int>> p5 = matrix_multiply(addMatrices(a11, a22), addMatrices(b11, b22), count);
    vector<vector<int>> p6 = matrix_multiply(subtractMatrices(a12, a22), addMatrices(b21, b22), count);
    vector<vector<int>> p7 = matrix_multiply(subtractMatrices(a11, a21), addMatrices(b11, b12), count);

    // finall result submatrices
    vector<vector<int>> c11 = addMatrices(addMatrices(p5, p4), subtractMatrices(p6, p2));
    vector<vector<int>> c12 = addMatrices(p1, p2);
    vector<vector<int>> c21 = addMatrices(p3, p4);
    vector<vector<int>> c22 = subtractMatrices(subtractMatrices(addMatrices(p5, p1), p3), p7);

    // result matrix
    for (int i = 0; i < mid; i++)
    {
        for (int j = 0; j < mid; j++)
        {
            result[i][j] = c11[i][j];
            result[i][j + mid] = c12[i][j];
            result[i + mid][j] = c21[i][j];
            result[i + mid][j + mid] = c22[i][j];
        }
    }
    return result;
}

void display_matrix(vector<vector<int>> m)
{
    for (int i = 0; i < m.size(); i++)
    {
        for (int j = 0; j < m[0].size(); j++)
        {
            cout << m[i][j] << " ";
        }
        cout << "\n";
    }
}

int main()
{
    srand(time(NULL));
    ofstream MMcount("MatrixMultiplication.csv");

    int bfcount = 0;
    int strassenscount = 0;

    vector<vector<int>> A{{1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}};
    vector<vector<int>> B{{1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}};

    vector<vector<int>> result = matrix_multiply(A, B, strassenscount);
    vector<vector<int>> result2 = bfmm(A, B, bfcount);
    cout << "BFcount: " << bfcount << "\n";
    cout << "StrassensCount: " << strassenscount << "\n";
    cout << "\n";

    display_matrix(result);
    cout << "\n";
    display_matrix(result2);

    //------------------------------------------------------File output testing-----------------------------

    MMcount << "BF" << "," << "Strassens" << "\n";
    for (int n = 16; n <= 256; n *= 2) // test for matrix sizes of 2^4 to 2^10
    {
        bfcount = 0;
        strassenscount = 0;
        vector<vector<int>> T1(n, vector<int>(n));
        vector<vector<int>> T2(n, vector<int>(n));

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                T1[i][j] = rand() % 50+1;
                T2[i][j] = rand() % 50+1;
            }
        }

        matrix_multiply(T1, T2, strassenscount);
        bfmm(T1, T2, bfcount);
        MMcount << bfcount << "," << strassenscount << "\n";
    }
}
