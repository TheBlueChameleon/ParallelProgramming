#include "mpiHelpers.hpp"
#include "00-tasks.hpp"

// ========================================================================== //

int main (int argc, char ** argv) {
    MPI_Setup(argc, argv);

//     task01_gather();
//     task02_allgatherv();
    task02_scatter();
}
