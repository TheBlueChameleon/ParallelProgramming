#include <mpi.h>

#include "mpiHelpers.hpp"
#include "00-tasks.hpp"

// ========================================================================== //

int main (int argc, char ** argv) {
    MPI_Setup(argc, argv);

    task01_reduce();
    MPI_Barrier(MPI::COMM_WORLD);
    task02_typedScatter();
    MPI_Barrier(MPI::COMM_WORLD);
    task03_typedSend();
}
