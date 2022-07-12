#include "mpiHelpers.hpp"
#include "00-tasks.hpp"

// ========================================================================== //

int main (int argc, char ** argv) {
    MPI_Setup(argc, argv);

//     task01_nbRecv();
//     task02_nbSend();
//     task03_nbDeadlock();
    task04_sendVarSize();
}
