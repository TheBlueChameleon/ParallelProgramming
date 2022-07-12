#include <vector>

#include <mpi.h>
#include "mpiHelpers.hpp"

void task02_scatter() {
    const int root = 0,
              N    = 2;

    std::vector<int> data;
    std::vector<int> recv(N, -1);

    if (procID == root) {
        data = decltype(data)(N * Nprocs - 2);
        for (auto& x : data) {x = getRandomIntBetween(0, 100);}

        std::cout << "Process root is ready to send:" << std::endl;
        for (const auto& x : data) {std::cout << "  " << x;}
        std::cout << std::endl;
    }

    MPI_Scatter(data.data(), N, MPI_INTEGER,
                recv.data(), N, MPI_INTEGER,
                root, MPI::COMM_WORLD);

    std::stringstream sbuffer;
    sbuffer << "Process #" << procID << " got share:";
    for (const auto& x : recv) {sbuffer << "  " << x;}
    sbuffer << std::endl;
    showAndClear(sbuffer);
}
