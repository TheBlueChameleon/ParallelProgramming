#include "mpiHelpers.hpp"
#include "00-tasks.hpp"

// ========================================================================== //

int main (int argc, char ** argv) {
    MPI_Setup(argc, argv);

    task01_pingpong();
    task02_chineseWhispers();
    task03_globalsSum();
}
