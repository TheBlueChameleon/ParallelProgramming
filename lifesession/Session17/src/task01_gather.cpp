#include <vector>

#include <mpi.h>
#include "mpiHelpers.hpp"

void task01_gather() {
    int error;

    const int root = 0;

    const int N = 1;
    std::vector<int> vec(N, procID),
                     all(Nprocs);

    error = MPI_Gather (vec.data(), N, MPI_INTEGER,
                        all.data(), N, MPI_INTEGER,
                        root, MPI::COMM_WORLD);

    if (procID == root) {
        std::cout << "root after gather received:" << std::endl;
        for (const auto x : all) {
            std::cout << "  " << x;
        }
        std::cout << std::endl;
    }
}
