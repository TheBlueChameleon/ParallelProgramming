#include <chrono>
#include <iostream>
#include <sstream>

#include <mpi.h>
#include "mpiHelpers.hpp"

// ========================================================================== //
// MPI Interface Utility

int procID, Nprocs;

void finalize() {
    const auto err = MPI_Finalize();

    if (err) {
        std::cout << "Note: Error #" << err << " in process #" << procID << " during shutdown of MPI." << std::endl;
    }
}

void MPI_Setup (int argc, char ** argv, bool verbose) {
    int err;
    err = MPI_Init(&argc, &argv);
    if (err) {MPI_Abort(MPI::COMM_WORLD, -1);}

    err = MPI_Comm_rank(MPI::COMM_WORLD, &procID);
    if (err) {MPI_Abort(MPI::COMM_WORLD, -1);}

    err = MPI_Comm_size(MPI::COMM_WORLD, &Nprocs);
    if (err) {MPI_Abort(MPI::COMM_WORLD, -1);}

    if (verbose) {
        std::stringstream buffer;
        buffer << "Process #" << procID << "/" << Nprocs << ": ready." << std::endl;
        std::cout << buffer.str();
    }

    std::atexit(finalize);
}

// ========================================================================== //
// Timing Utility

std::chrono::high_resolution_clock timeDevice;

timepoint_t getNow() {return timeDevice.now();}

double getTimeBetweenInMilliSeconds(timepoint_t tic, timepoint_t toc) {
    const timespan_t delta = toc - tic;
    return delta.count();
}

double getTimeSinceInMilliSeconds(timepoint_t tic) {
    const timespan_t delta = timeDevice.now() - tic;
    return delta.count();
}

// ========================================================================== //
// Randm Number Utility

std::random_device               _random_device;
std::mt19937                     _random_generator(_random_device());
std::uniform_real_distribution<> _random_distribution(0., 1.);

double getRandomBetween(const double a, const double b) {
    double p = _random_distribution(_random_generator);
    return p * (b - a) + a;
}

int getRandomIntBetween(const int a, const int b) {
    std::uniform_int_distribution<> distrib(a, b);
    return distrib(_random_generator);
}

// ========================================================================== //
// Misc

void showAndClear(std::stringstream& buffer) {
    std::cout << buffer.str();
    buffer.str("");
}
