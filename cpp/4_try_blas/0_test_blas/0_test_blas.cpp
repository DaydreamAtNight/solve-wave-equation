#include <iostream>
#include <cstdlib>
using namespace std;

// A convenient macro to fix differences in symbol names between C and Fortran
#define F77NAME(x) x##_

// Function prototypes for the BLAS functions we will use
// Tell the compiler to use "C"-style symbol names (without name mangling)
extern "C" {
    double F77NAME(ddot) (
        const int& n, const double *x, const int & incx,
        const double *y, const int& incy
    );
}

extern "C" {
    void F77NAME(dgemm) (
        const char& transA, const char& transB,
        const int& m, const int& n,
        const int& k, const double& alpha,
        const double* A, const int& lda,
        const double* B, const int& ldb,
        const double& beta, double* C,
        const int& ldc
    );
}

extern "C" {
    void F77NAME(dsymm) (
        const char& side, const char& uplo,
        const int& m, const int& n,
        const double& alpha,
        const double* A, const int& lda,
        const double* B, const int& ldb,
        const double& beta, 
        double* C, const int& ldc
    );
}


void print_matrix(const int M, const int N, const double* C)
{
    for(int i = 0; i<M; i++)
    {
        for(int j = 0; j<N; j++)
        {
            cout << C[i*N+j]<< " " ;
        }
        cout << endl;
    }
}

void print_vec(const int len, const double* C)
{
    for(int i = 0; i<len; i++)
    {
        cout << C[i] << " " ;  
    }
    cout << endl;
}


void test_array_level1()
{
    const int N = 64;
    double* x = new double[N];
    double* y = new double[N];
    double z = 0.0;

    srand(time(0));
    for(int i = 0; i<N; i++)
    {
        x[i] = (double) (rand()) / RAND_MAX;
        y[i] = (double) (rand()) / RAND_MAX;
    }

    z = F77NAME(ddot) (N, x, 1, y, 1);
    cout << "Dot product of x and y is: "<< z << endl;

    delete[] x;
    delete[] y;
}

void test_array_level3_normal()
{
    const int M = 2;               // Number of rows of A
    const int K = M;               // Number of rows of B
    const int N = 4;               // Number of columns of B, C
    double alpha = 1.0;
    double beta = 1.0;
    double* AI= new double[M*K];  // identical matrix
    double* B = new double[K*N];
    double* C = new double[M*N];

    // populate matrix
    double d = 0;
    for(int i = 0; i<M; i++)
    {
        AI[i+i*K] = 1;
        for(int j = 0; j<N; j++)
        {
            B[i*N+j] = d;
            C[i*N+j] = d;
            d++;
        }
    }

    print_matrix(M, K, AI);
    cout << "==============" << endl;
    print_matrix(K, N, B);
    cout << "==============" << endl;
    print_matrix(M, N, C);
    cout << "==============" << endl;

    print_vec(K*N, B);                 // test row major storage
    cout << "==============" << endl;
    print_vec(M*N, C);                 // test row major storage
    cout << "==============" << endl;

    // simply C = B+C
    F77NAME(dgemm) ('N', 'N', M, N, K, alpha, AI, K, B, K, beta, C, M);

    print_matrix(M, N, C);

    delete[] AI;
    delete[] B;
    delete[] C;
}


void test_array_level3_sym()
{
    const int M = 3;               // Number of rows of A
    const int K = M;               // Number of rows of B
    const int N = 4;               // Number of columns of B, C
    double alpha = 1.0;
    double beta = 1.0;
    double* SYMAI= new double[((1+M)*M)/2];  // identical matrix
    double* B = new double[K*N];
    double* C = new double[M*N];

    // populate matrix
    int index = 0;
    for (int i=M; i>0; i--)
    {
        for(int j = 0; j<i; j++)
        {
            if (j==0)
            {
                SYMAI[index] = 1;
            }
            else 
            {
                SYMAI[index] = 0;
            }
            index++;
        }
    }

    double d = 0;
    for(int i = 0; i<M; i++)
    {
        for(int j = 0; j<N; j++)
        {
            B[i*N+j] = d;
            C[i*N+j] = d;
            d++;
        }
    }

    print_vec(((1+M)*M)/2, SYMAI);
    cout << "==============" << endl;
    // print_matrix(K, N, B);
    // cout << "==============" << endl;
    // print_matrix(M, N, C);
    // cout << "==============" << endl;

    // print_vec(K*N, B);                 // test row major storage
    // cout << "==============" << endl;
    // print_vec(M*N, C);                 // test row major storage
    // cout << "==============" << endl;

    // // simply C = B+C
    // // F77NAME(dgemm) ('N', 'N', M, N, K, alpha, SYMAI, K, B, K, beta, C, M);

    // print_matrix(M, N, C);

    // delete[] SYMAI;
    // delete[] B;
    // delete[] C;
}


int main()
{
    // test_array_level1();
    // test_array_level3_normal();
    test_array_level3_sym();
}

// compile with g++ --std=c++11 0_test_blas.cpp -lblas
// if cannot find blas:
// // rpm -ql blas
// // /usr/lib64/libblas.so.3
// // /usr/lib64/libblas.so.3.4
// // /usr/lib64/libblas.so.3.4.2
// // /usr/share/doc/blas-3.4.2
// // /usr/share/doc/blas-3.4.2/LICENSE
// // /usr/share/doc/blas-3.4.2/blasqr.ps
// // cd /usr/lib64/
// // ln -s libblas.so.3 libblas.so