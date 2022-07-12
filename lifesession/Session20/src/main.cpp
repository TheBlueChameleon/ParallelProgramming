#include <mpi.h>

#include "mpiHelpers.hpp"
#include "00-tasks.hpp"

// ========================================================================== //

int main (int argc, char ** argv) {
    MPI_Setup(argc, argv);

//     task01_splitComm();
//     task02_groupComm();
    task03_IO();
}
