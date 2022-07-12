#include <vector>

#include <mpi.h>
#include "mpiHelpers.hpp"

void task02_typedScatter() {
    const int root = 0,
              N    = 4;

    std::vector<int> data;
    std::vector<int> recv(N, -1);

    if (procID == root) {
        std::cout << "================================================" << std::endl;

        data = decltype(data)(N * Nprocs);
        for (auto& x : data) {x = getRandomIntBetween(0, 100);}

        std::cout << "Process root is ready to send:" << std::endl;
        for (const auto& x : data) {std::cout << "  " << x;}
        std::cout << std::endl;
    }

    MPI_Datatype two_type;
    MPI_Type_contiguous(2, MPI_INTEGER, &two_type);
    MPI_Type_commit(&two_type);

    MPI_Scatter(data.data(), N  , MPI_INTEGER,
                recv.data(), N/2, two_type,
                root, MPI::COMM_WORLD);

    std::stringstream sbuffer;
    sbuffer << "Process #" << procID << " got share:";
    for (const auto& x : recv) {sbuffer << "  " << x;}
    sbuffer << std::endl;
    showAndClear(sbuffer);

    MPI_Type_free(&two_type);
}
