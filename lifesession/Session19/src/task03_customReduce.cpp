#include <cstddef>
#include <sstream>

#include <mpi.h>

#include "mpiHelpers.hpp"

struct Transfer_t {
    int     a, b;
    float   c, d;
};

void sum_Transfer_t (void *pElm, void *pAcc, int *N, [[maybe_unused]] MPI_Datatype *datatype) {
    Transfer_t *elm = static_cast<Transfer_t *>(pElm),
               *acc = static_cast<Transfer_t *>(pAcc);

    for (int i=0; i<*N; ++i) {
        acc[i].a += elm[i].a;
        acc[i].b += elm[i].b;
        acc[i].c += elm[i].c;
        acc[i].d += elm[i].d;
    }
}

void task03_customReduce() {
    const int other = 1 - procID;
    const int tag   = 0;

    std::stringstream sBuffer;

    MPI_Datatype MPIX_Transfer;

    const int          blockLengths [] = {2, 2};
    const long int     displacements[] = {offsetof(Transfer_t, a), offsetof(Transfer_t, c)};
    const MPI_Datatype blockTypes   [] = {MPI_INTEGER, MPI_FLOAT};

    MPI_Type_create_struct(2, blockLengths, displacements, blockTypes, &MPIX_Transfer);
    MPI_Type_commit(&MPIX_Transfer);

    MPI_Op MPIX_Transfer_reduce_sum;
    MPI_Op_create(sum_Transfer_t, true, &MPIX_Transfer_reduce_sum);

    const int N = 2;
    Transfer_t *source = new Transfer_t[N],
               *total  = new Transfer_t[N];

    for (int i = 0; i < N; ++i) {
        source[i] = { 1  , 2,
                      0.1, 0.2};
    }

    MPI_Allreduce(source, total, N, MPIX_Transfer, MPIX_Transfer_reduce_sum, MPI::COMM_WORLD);

    for (int i = 0; i < N; ++i) {
        sBuffer << "process #" << procID << " received struct data at idx " << i << ":" << std::endl;
        sBuffer << "  " << total[i].a << ", " << total[i].b << std::endl;
        sBuffer << "  " << total[i].c << ", " << total[i].d << std::endl;
        showAndClear(sBuffer);
    }

    delete[] source;
    delete[] total;

}
