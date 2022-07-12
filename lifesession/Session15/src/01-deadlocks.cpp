#include <iostream>
#include <sstream>

#include "mpi.h"
#include "mpiHelpers.hpp"

void task01_deadlocks() {
    const size_t N = 10'000;
    const int tag = 0;

    std::stringstream sbuf;

    int other = 1 - procID;

    int * data = new int[N];

    if          (procID == 0) {
        for (size_t i = 0; i < N; ++i) {data[i] = 1;}

        MPI_Send(data, N, MPI_INTEGER, other, tag, MPI::COMM_WORLD);
        sbuf << "proc #" << procID << " sent:" << std::endl;
        sbuf << "  " << data[0] << ", ...";
        sbuf << std::endl;
        std::cout << sbuf.str();
        sbuf.str("");

        MPI_Recv(data, N, MPI_INTEGER, other, tag, MPI::COMM_WORLD, MPI_STATUS_IGNORE);
        sbuf << "proc #" << procID << " received:" << std::endl;
        sbuf << "  " << data[0] << ", ...";
        sbuf << std::endl;
        std::cout << sbuf.str();
        sbuf.str("");

    } else if   (procID == 1) {
        MPI_Recv(data, N, MPI_INTEGER, other, tag, MPI::COMM_WORLD, MPI_STATUS_IGNORE);
        sbuf << "proc #" << procID << " received:" << std::endl;
        sbuf << "  " << data[0] << ", ...";
        sbuf << std::endl;
        std::cout << sbuf.str();
        sbuf.str("");

        for (size_t i = 0; i < N; ++i) {data[i] = 0;}

        MPI_Send(data, N, MPI_INTEGER, other, tag, MPI::COMM_WORLD);
        sbuf << "proc #" << procID << " sent:" << std::endl;
        sbuf << "  " << data[0] << ", ...";
        sbuf << std::endl;
        std::cout << sbuf.str();
        sbuf.str("");
    }

    MPI_Barrier(MPI::COMM_WORLD);

    if          (procID == 0) {
        for (size_t i = 0; i < N; ++i) {data[i] = 1;}
    } else if   (procID == 1) {
        for (size_t i = 0; i < N; ++i) {data[i] = 0;}
    }

    MPI_Sendrecv(data , N, MPI_INTEGER, other, tag,
                 data , N, MPI_INTEGER, other, tag,
                 MPI::COMM_WORLD, MPI_STATUS_IGNORE);

    sbuf << "proc #" << procID << " after MPI_Sendrecv:" << std::endl;
    sbuf << "  " << data[0] << ", ...";
    sbuf << std::endl;
    std::cout << sbuf.str();
    sbuf.str("");

    std::cout << "endl" << std::endl;

    delete data;
}
