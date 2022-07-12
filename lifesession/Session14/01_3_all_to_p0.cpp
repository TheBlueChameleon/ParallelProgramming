#include <cstdlib>
#include <iostream>
#include <vector>

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

    const int tag = 0, master = 0;
    int data;

    if (procID == master) {
        MPI_Status status;
        std::vector<int> replies(Nprocs);

        std::cout << "master process about to receive ... " << std::flush;

        for (auto receiver = 1; receiver < Nprocs; ++receiver) {
            MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI::COMM_WORLD, &status);
            replies[status.MPI_SOURCE] = data;
        }

        std::cout << "done." << std::endl;

        for (const auto reply : replies) {
            std::cout << reply << std::endl;
        }

    } else {
        data = procID;

        std::cout << "process " << procID << " is sending data = " << data << " ... " << std::flush;
            MPI_Send(&data, 1, MPI_INT, master, tag, MPI::COMM_WORLD);
        std::cout << "preceived: data = " << data << std::endl;
    }
}
