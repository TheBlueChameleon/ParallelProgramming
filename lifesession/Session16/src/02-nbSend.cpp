#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std::chrono_literals;

#include "mpi.h"
#include "mpiHelpers.hpp"

void task02_nbSend() {
    const int other = 1 - procID;
    const int tag   = 0;

    const int   N = 4;
    int * const data = new int[N];

    std::stringstream sbuffer;

    if (procID == 0) {
        MPI_Request request;
        MPI_Status status;

        for (int i = 0; i < N; ++i) {data[i] = i;}

        MPI_Isend(data, N, MPI_INTEGER,
                  other, tag,
                  MPI::COMM_WORLD, &request);
        MPI_Wait(&request, &status);

        sbuffer << "Process #" << procID << " sent data." << std::endl;
        std::cout << sbuffer.str();
        sbuffer.str("");


    } else if (procID == 1) {
        std::this_thread::sleep_for(10ms);

        MPI_Status status;
        MPI_Recv(data, N, MPI_INTEGER,
                 other, tag,
                 MPI::COMM_WORLD, &status);

        sbuffer << "Process #" << procID << " completed to receiving data:" << std::endl << "\t";
        for (int i = 0; i < N; ++i) {sbuffer << data[i] << " ";}
        sbuffer << std::endl;
        std::cout << sbuffer.str();
        sbuffer.str("");
    }

    delete data;
}
