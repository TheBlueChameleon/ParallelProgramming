#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

#include <mpi.h>
#include "mpiHelpers.hpp"

int* getMatrixProduct(size_t rowsA, size_t colsA, int* A,
                      size_t rowsB, size_t colsB, int* B)
{
    if (colsA != rowsB) {throw std::runtime_error("Incompatible matrices");}

    int * C = new int[rowsA * colsB];
    size_t n;

    for         (auto i=0u; i<rowsA; ++i) {
        for     (auto j=0u; j<colsB; ++j) {
            n = i*colsB + j;
            C[n] = 0;
            for (auto k=0u; k<rowsB; ++k) {
                C[n] += A[i*colsA + k] * B[k*colsB + j];
            }
        }
    }

    return C;
}

void printMatrix (size_t rows, size_t cols, int* M, const std::string& head) {
    std::stringstream s;

    s << std::fixed << std::setw(2);
    s << head << std::endl;
    for         (auto i=0u; i<rows; ++i) {
        for     (auto j=0u; j<cols; ++j) {
            s  << std::setw(2) << M[i*cols + j] << "  ";
        }
        s << std::endl;
    }
    s << std::endl;

    showAndClear(s);
}

int getComponentA(size_t i, size_t j) {return (i&1) + (j&2) + 1;}
int getComponentB(size_t i, size_t j) {return (i&1) + (j&1)    ;}

void task01_matmul() {
    const int root = 0;
    const int tag = 0;
    const size_t N = Nprocs << 3;       // matrix size N x N; limit to "nice cases" where the matrix can be distributed evenly across processes
    const int debug = 0;

    std::string head;

    // define A
    int * A = new int[N * N];
    for (auto i=0u; i<N*N; ++i) {A[i] = getComponentA(i, i/N);}

    // main scope variables for later deallocation; only used in the control blocks
    int * B_full;
    int * C_expected;

    // create control result
    if (procID == root) {
        B_full = new int[N * N];

        for (auto i=0u; i<N*N; ++i) {B_full[i] = getComponentB(i, i/N);}

        C_expected = getMatrixProduct(N, N, A,
                                      N, N, B_full);

        if (debug > 3) {
            printMatrix(N, N, A         , "A:");
            printMatrix(N, N, B_full    , "B:");
            printMatrix(N, N, C_expected, "C:");
        }
    }

    // process-local part
    const size_t sliceN = N / Nprocs;
    const size_t colOffset = procID * sliceN;

    // construct local B
    int * B = new int[N * sliceN];
    for     (auto i=0u; i<N     ; ++i) {
        for (auto j=0u; j<sliceN; ++j) {
            B[i*sliceN + j] = getComponentB(i, j + colOffset);
        }
    }

    if (debug > 2) {
        head = "A in process #" + std::to_string(procID);
        printMatrix(N, N, A, head);

        head = "B in process #" + std::to_string(procID);
        printMatrix(N, sliceN, B, head);
    }

    // get local slice of result matrix
    int * C = getMatrixProduct(N,      N, A,
                               N, sliceN, B);

    if (debug > 0) {
        head = "C in process #" + std::to_string(procID);
        printMatrix(N, sliceN, C, head);
    }

    // prepare communication
    int * C_collected = nullptr;
    if (procID == root) {C_collected = new int[N*N];}

    MPI_Datatype matrixColumn_t;
    MPI_Type_vector(N, sliceN, N, MPI_INTEGER, &matrixColumn_t);
    MPI_Type_commit(&matrixColumn_t);

    // ... manually send local slice
    MPI_Request req;
    MPI_Isend(C, N*sliceN, MPI_INTEGER,
              root, tag,
              MPI::COMM_WORLD, &req);

    // manually receive slice and put into correct slot in result matrix
    if (procID == root) {
        MPI_Status status;
        for (auto i=0u; i< Nprocs; ++i) {
            MPI_Recv(C_collected + i * sliceN,
                     1, matrixColumn_t,
                     i, tag,
                     MPI::COMM_WORLD, &status);
        }
    }

    // output and check
    if (procID == root) {
        printMatrix(N, N, C_expected, "C, expected:");

        head = "C collected in process #" + std::to_string(procID);
        printMatrix(N, N, C_collected, head);

        if (std::memcmp(C_expected, C_collected, N*N))  {std::cout << "discrepancy detected." << std::endl;}
        else                                            {std::cout << "results agree."        << std::endl;}
    }

    // tidy up ...
    delete[] A;
    delete[] B;
    delete[] C;

    // ... incl root-local control block vars
    if (procID == root) {
        delete[] B_full;
        delete[] C_collected;
        delete[] C_expected;
    }
}
