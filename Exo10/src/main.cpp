#include <mpi.h>

#include "mpiHelpers.hpp"
#include "00-tasks.hpp"

// ========================================================================== //

int main (int argc, char ** argv) {
    MPI_Setup(argc, argv);

    MPI_Barrier(MPI::COMM_WORLD);
    task01_matmul();
}
