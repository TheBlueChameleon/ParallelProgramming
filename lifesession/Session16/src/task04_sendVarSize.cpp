#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

#include "mpi.h"
#include "mpiHelpers.hpp"

void task04_sendVarSize() {
    const int other = 1 - procID,
              tag   = 0;

    MPI_Status status;

    int N;
    int * data;

    std::stringstream sbuffer;

    std::srand( std::time(nullptr) );

    if          (procID == 0) {
        N = 1 + std::rand() % 10;

        data = new int[N];
        for (int i=0; i<N; ++i) {data[i] = i+1;}

        MPI_Send(data, N, MPI_INTEGER, other, tag, MPI::COMM_WORLD);
        std::cout << "completed sending " << N << " ints" << std::endl;

    } else if   (procID == 1) {
        MPI_Probe(other, tag,
                  MPI::COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INTEGER, &N);


        data = new int[N];
        MPI_Recv(data, N, MPI_INTEGER,
                 other, tag,
                 MPI::COMM_WORLD, &status);

        sbuffer  << "received " << N << " ints:" << std::endl;
        for (int i = 0; i < N; ++i) {sbuffer << data[i] << " ";}
        sbuffer << std::endl;
        std::cout << sbuffer.str();
        sbuffer.str("");
    }

    delete [] data;
}
