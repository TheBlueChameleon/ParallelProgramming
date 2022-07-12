#include <vector>

#include <mpi.h>
#include "mpiHelpers.hpp"

void task02_allgatherv() {
    int error;

    const int root = 0;

    const int N      = 2,
              stride = 3;
    std::vector<int> vec            (N, procID + 1),
                     all            (Nprocs * stride, 0),
                     counts         (Nprocs, N),
                     displacements  (Nprocs);

    for (int i=0; auto& d : displacements) {
        d = i * stride;
        ++i;
    }

    error = MPI_Allgatherv (vec.data(), N, MPI_INTEGER,
                            all.data(), counts.data(), displacements.data(), MPI_INTEGER,
                            MPI::COMM_WORLD);

    if (procID == root) {
        std::cout << "root after gather (1) received:" << std::endl;
        for (const auto x : all) {
            std::cout << "  " << x;
        }
        std::cout << std::endl;
    }

    const size_t totalDataSize = Nprocs * (Nprocs + 1) * .5;
    vec.resize(procID + 1);
    for (auto& v : vec) {v = procID + 1;}
    all.resize(totalDataSize);

    for (int i=0; auto& c : counts) {c = ++i;}
    for (int i=0; auto& d : displacements) {
        int last;
        last += i;
        d = last;
        ++i;
    }
    error = MPI_Allgatherv (vec.data(), counts[procID], MPI_INTEGER,
                            all.data(), counts.data(), displacements.data(), MPI_INTEGER,
                            MPI::COMM_WORLD);

    if (procID == root) {
        std::cout << "root after gather (2) received:" << std::endl;
        for (const auto x : all) {
            std::cout << "  " << x;
        }
        std::cout << std::endl;

    }
}
