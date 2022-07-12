#include <iostream>
#include <sstream>

#include "mpi.h"
#include "mpiHelpers.hpp"

void task03_nbDeadlock() {
    const int other = 1 - procID,
              tagA  = 0,
              tagB  = 1;
    int dataOut = procID;
    int dataIn;

    MPI_Status status;
    MPI_Request request;

    std::stringstream sbuffer;

    if          (procID == 0) {
        MPI_Isend(&dataOut, 1, MPI_INTEGER,
                  other, tagA,
                  MPI_COMM_WORLD, &request);
        MPI_Recv(&dataIn, 1, MPI_INTEGER,
                 other, tagB,
                 MPI::COMM_WORLD, &status);

    } else if   (procID == 1) {
        MPI_Isend(&dataOut, 1, MPI_INTEGER,
                  other, tagB,
                  MPI_COMM_WORLD, &request);
        MPI_Recv(&dataIn, 1, MPI_INTEGER,
                 other, tagA,
                 MPI::COMM_WORLD, &status);
    }

    sbuffer << "Process #" << procID << " completed exchange:" << std::endl;
    sbuffer << "  out:" << dataOut << std::endl;
    sbuffer << "  in :" << dataIn  << std::endl;
    std::cout << sbuffer.str();
    sbuffer.str("");
}
