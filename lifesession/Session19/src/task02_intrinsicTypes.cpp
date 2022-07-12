#include <cstddef>
#include <sstream>

#include <mpi.h>

#include "mpiHelpers.hpp"

struct Transfer_t {
    int     a, b;
    float   c, d;
};

void task02_intrinsicTypes() {
    const int other = 1 - procID;
    const int tag   = 0;

    std::stringstream sBuffer;

    MPI_Datatype MPIX_Transfer;

    const int          blockLengths [] = {2, 2};
    const long int     displacements[] = {offsetof(Transfer_t, a), offsetof(Transfer_t, c)};
    const MPI_Datatype blockTypes   [] = {MPI_INTEGER, MPI_FLOAT};

    MPI_Type_create_struct(2, blockLengths, displacements, blockTypes, &MPIX_Transfer);
    MPI_Type_commit(&MPIX_Transfer);

    if          (procID == 0) {
        Transfer_t data = { 1  , 2,
                            0.1, 0.2};
        MPI_Send(&data, 1, MPIX_Transfer, other, tag, MPI::COMM_WORLD);

        sBuffer << "process #" << procID << " sent struct data." << std::endl;
        showAndClear(sBuffer);

    } else if   (procID == 1) {
        MPI_Status status;
        Transfer_t data;

        MPI_Recv(&data, 1, MPIX_Transfer, other, tag, MPI::COMM_WORLD, &status);

        sBuffer << "process #" << procID << " received struct data:" << std::endl;
        sBuffer << "  " << data.a << ", " << data.b << std::endl;
        sBuffer << "  " << data.c << ", " << data.d << std::endl;
        sBuffer << "=============================================" << std::endl;
        showAndClear(sBuffer);
    }
}
