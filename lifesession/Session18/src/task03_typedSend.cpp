#include <sstream>
#include <vector>

#include <mpi.h>
#include "mpiHelpers.hpp"

#include <vector>

void task03_typedSend() {
    const int sender = 0,
              receiver = 1;
    const int tag = 0;
    std::stringstream buffer;

    if          (procID == sender) {
        std::cout << "================================================" << std::endl;

        std::vector<int> data = {1,2,3,4,5,6};
        const int blocklengths [2] = {2, 2};
        const int displacements[2] = {0, 3};

        MPI_Datatype pos_selected_t;
        MPI_Type_indexed(2, blocklengths, displacements, MPI_INTEGER, &pos_selected_t);
        MPI_Type_commit(&pos_selected_t);

        buffer << "sender (process #" << procID << ") is ready to send from:" << std::endl;
        for (const auto& x : data) {
            buffer << "  " << x;
        }
        buffer << std::endl;
        showAndClear(buffer);

        MPI_Send(data.data(), 1, pos_selected_t,
                 receiver, tag, MPI::COMM_WORLD);


    } else if   (procID == receiver) {
        std::vector<int> data(4);
        MPI_Recv(data.data(), data.size(), MPI_INTEGER, sender, tag,
                 MPI::COMM_WORLD, MPI_STATUS_IGNORE);

        buffer << "receiver (process #" << procID << ") is got:" << std::endl;
        for (const auto& x : data) {
            buffer << "  " << x;
        }
        buffer << std::endl;
        showAndClear(buffer);
    }
}
