#include <iostream>
#include <sstream>

#include <mpi.h>

int procID, Nprocs;

void finalize() {
    const auto err = MPI_Finalize();

    if (err) {
        std::cout << "Note: Error #" << err << " during shutdown of MPI." << std::endl;
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
