#include <algorithm>
#include <bit>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include <mpi.h>
#include "mpiHelpers.hpp"

void flipBit            (      int& x, const int bitIndex)  {x ^= (1 << bitIndex);}
int getWithFlippedBit   (const int  x, const int bitIndex)  {return x ^ (1 << bitIndex);}

std::vector<int> getNeighbours() {
    std::vector<int> reVal;

    assert( std::popcount<unsigned int>(Nprocs) == 1 );                         // make sure this runs with 2**k processes
    const size_t bit_width = std::bit_width<unsigned int>(Nprocs) - 1;

    for (auto i=0u; i<bit_width; ++i) {
        reVal.push_back(getWithFlippedBit(procID, i));
    }

    return reVal;
}

void task03_globalsSum() {
    std::stringstream sbuffer;

    const size_t N = 10;
    std::vector<int> data(N), buffer(N);
    for (auto& x : data) {x = getRandomIntBetween(0, 10);}

    sbuffer << "Process #" << procID << " has numbers:" << std::endl;
    for (const auto& x : data) {sbuffer << "  " << x;}
    sbuffer << std::endl;
    showAndClear(sbuffer);

    auto neighbours = getNeighbours();
    //     sbuffer << "Process #" << procID << " has getNeighbours:" << std::endl;
    //     for (const auto& x : neighbours) {sbuffer << "  " << x;}
    //     sbuffer << std::endl;
    //     showAndClear(sbuffer);

    const int tag = 0;
    MPI_Status status;
    for (const auto& neighbour : neighbours) {
        MPI_Sendrecv(data  .data(), N, MPI_INTEGER, neighbour, tag,
                     buffer.data(), N, MPI_INTEGER, neighbour, tag,
                     MPI::COMM_WORLD, &status);

        std::transform(data  .begin(), data.end(),
                       buffer.begin(),
                       data  .begin(),
                       std::plus<>()
        );
    }

    sbuffer << "Process #" << procID << " computed numbers:" << std::endl;
    for (const auto& x : data) {sbuffer << "  " << x;}
    sbuffer << std::endl;
    showAndClear(sbuffer);
}
