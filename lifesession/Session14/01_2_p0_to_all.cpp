#include <cstdlib>
#include <iostream>

#include <mpi.h>

int procID, Nprocs;

void finalize();
void setup (int argc, char ** argv) {
    auto err = MPI_Init(&argc, &argv);

    if (err) {
        MPI_Abort(MPI::COMM_WORLD, -1);
    }

    std::atexit(finalize);

    err =  MPI_Comm_rank(MPI::COMM_WORLD, &procID);
    err =  MPI_Comm_size(MPI::COMM_WORLD, &Nprocs);
}

void finalize() {
    MPI_Finalize();
}

int main (int argc, char ** argv) {
    setup(argc, argv);
    std::cout << procID << "/" << Nprocs << ": ready." << std::endl;

    const int tag = 0, sender = 0;
    int data;

    if (procID == sender) {
        data = -1;
        std::cout << "master process about to send data = " << data << " ... " << std::flush;

        for (auto receiver = 1; receiver < Nprocs; ++receiver) {
            MPI_Send(&data, 1, MPI_INT, receiver, tag, MPI::COMM_WORLD);
        }

        std::cout << "done." << std::endl;

    } else {
        MPI_Status status;

        std::cout << "process " << procID << " is listening for data ... " << std::flush;
        MPI_Recv(&data, 1, MPI_INT, sender, tag, MPI::COMM_WORLD, &status);
        std::cout << "preceived: data = " << data << std::endl;
    }
}
