#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "mpi.h"
#include "mpiHelpers.hpp"

using namespace std::chrono_literals;

void task02_rsend() {
    const size_t N = 10'000;
    const int tag = 0;

    std::stringstream sbuf;

    int other = 1 - procID;

    int * data = new int[N];

    if          (procID == 0) {
        for (size_t i = 0; i < N; ++i) {data[i] = 42;}
        MPI_Rsend(data, N, MPI_INTEGER, other, tag, MPI::COMM_WORLD);

        sbuf << "proc #" << procID << " sent:" << std::endl;
        sbuf << "  " << data[0] << ", ...";
        sbuf << std::endl;
        std::cout << sbuf.str();
        sbuf.str("");

    } else if   (procID == 1) {
        std::this_thread::sleep_for(10ms);
        MPI_Recv(data, N, MPI_INTEGER, other, tag, MPI::COMM_WORLD, MPI_STATUS_IGNORE);

        sbuf << "proc #" << procID << " received:" << std::endl;
        sbuf << "  " << data[0] << ", ...";
        sbuf << std::endl;
        std::cout << sbuf.str();
        sbuf.str("");
    }

    delete data;
}
