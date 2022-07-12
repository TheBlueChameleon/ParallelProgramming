#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include <mpi.h>

#include "mpiHelpers.hpp"

void task01_splitComm() {
    const int root = 0;
    const int debug = 1;

    std::stringstream s;

    const int color = procID & 1;
    const int key   = procID >> 1;

    MPI_Comm commEO;

    MPI_Comm_split(MPI::COMM_WORLD, color, key, &commEO);

    if (debug)  {
        s << "(01) world process #" << procID << " is rank #" << key << " in group " << color << std::endl;
        showAndClear(s);
    }

    int msg = -1;
    if (procID == root) {
        msg = 1;
    }

    MPI_Bcast(static_cast<void*>(&msg),
              1, MPI_INTEGER,
              root, commEO);

    if (color == 0) {std::this_thread::sleep_for(  std::chrono::milliseconds(10)  );}
    MPI_Barrier(commEO);

    s << "(01) world process #" << procID << " (rank #" << key << " in group " << color << ") received " << msg << "." << std::endl;
        showAndClear(s);
}
