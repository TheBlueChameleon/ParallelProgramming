#include <iostream>
#include <string>

#include <mpi.h>
#include "mpiHelpers.hpp"

const std::string defaultMessage = "MPI is great!";

void task02_chineseWhispers() {
    const auto next = (procID + 1) % Nprocs;
    const auto prev = procID ? procID - 1 : MPI_PROC_NULL;
    const auto size = defaultMessage.size();

    const int tag = 0;
    MPI_Status status;

    std::string message(size, '?');

    MPI_Recv(message.data(), size, MPI_CHAR,
                  prev, tag,
                  MPI::COMM_WORLD, &status);

    if (procID) {
        const size_t pos = getRandomIntBetween(0, size);
        message[pos] = '?';

    } else {
        message = defaultMessage;
        std::cout << "Process #" << procID << " sent '" << message << "'" << std::endl;
    }

    MPI_Send(message.data(), size, MPI_CHAR,
                next, tag, MPI::COMM_WORLD);

    if (procID == 0) {
        MPI_Recv(message.data(), size, MPI_CHAR,
                  Nprocs - 1, tag,
                  MPI::COMM_WORLD, &status);
    }

    std::cout << "Process #" << procID << " got  '" << message << "'" << std::endl;
}
