#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include <mpi.h>
#include "mpiHelpers.hpp"

std::vector<size_t> getSizesVector(const size_t N) {
    std::vector<size_t> datasizes;
    size_t s = 1;

    for (int i=0; i<N+1; ++i) {
        datasizes.push_back(s);
        s <<= 1;
    }

    return datasizes;
}

double getPingpongTime(const size_t bytes) {
    const auto other = 1 - procID;
    const int  tag   = 0;

    MPI_Status status;

    // prepare message:
    std::vector<char> message(bytes);
    std::vector<char> answer (bytes);
    for (auto& b : message) {b = '=';}

    // start timing
    const auto tic = getNow();

    if          (procID == 0) {                                                 // ping ...
        MPI_Send(message.data(), bytes, MPI_CHAR,
                other, tag, MPI::COMM_WORLD);
        MPI_Recv(answer .data(), bytes, MPI_CHAR,
                  other, tag,
                  MPI::COMM_WORLD, &status);

    } else if   (procID == 1) {                                                 // ... pong
        MPI_Recv(answer .data(), bytes, MPI_CHAR,
                  other, tag,
                  MPI::COMM_WORLD, &status);
        MPI_Send(message.data(), bytes, MPI_CHAR,
                other, tag, MPI::COMM_WORLD);
    }


    // stop timing
    return getTimeSinceInMilliSeconds(tic);
}

void task01_pingpong() {
    const auto      datasizes   = getSizesVector(20);
    const size_t    repetitions = 10;

    double currentTimeBuffer;
    std::vector<double> times;
    for (const auto& bytes : datasizes) {

        currentTimeBuffer = 0;
        for (int i=0; i<repetitions; ++i) {
            currentTimeBuffer += getPingpongTime(bytes);
        }
        currentTimeBuffer /= repetitions;

        times.push_back(currentTimeBuffer);
    }

    if (procID == 0) {
        std::ofstream hFile("times_01.txt");

        std::cout << std::fixed;
        hFile     << std::fixed;
        for (auto i = 0u; i < datasizes.size(); ++i) {

            std::cout   << std::setw( 2) << i << ")\t"
                        << std::setw(10) << datasizes[i] << " Bytes in\t"
                        << std::setw(10) << std::setprecision(8)<< times    [i] << "ms" << std::endl;

            hFile       << std::setw( 2) << i << "\t"
                        << std::setw(10) << datasizes[i] << "\t"
                        << std::setw(10) << std::setprecision(8)<< times    [i] << std::endl;
        }
    }
}
