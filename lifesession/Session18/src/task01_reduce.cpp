#include <iostream>
#include <sstream>
#include <vector>

#include <mpi.h>
#include "mpiHelpers.hpp"

void task01_reduce() {
    int total;
    std::stringstream buffer;

    const int root = 0;
    const int N = 5;
    std::vector<int> numbers(N, procID),
                     results(N);

    MPI_Reduce(&procID, &total, 1, MPI_INTEGER, MPI_SUM,
               root, MPI::COMM_WORLD);

    if (procID == root) {
        buffer << "root process (#" << procID << ") found total = " << total << " using MPI_Reduce." << std::endl;
        showAndClear(buffer);
    }

    MPI_Allreduce(&procID, &total, 1, MPI_INTEGER, MPI_SUM, MPI::COMM_WORLD);
    buffer << "process #" << procID << " found total = " << total << " using MPI_Allreduce." << std::endl;
    showAndClear(buffer);

    MPI_Allreduce(numbers.data(), results.data(), N, MPI_INTEGER, MPI_SUM, MPI::COMM_WORLD);
    buffer << "process #" << procID << " found vector sum:" << std::endl;
    for (const auto x : results) {
        buffer << "  " << x;
    }
    buffer << std::endl;
    showAndClear(buffer);
}
