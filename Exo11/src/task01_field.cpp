// if you read this later on:
// this is column major, or phi[x, y] = phi[x*height + y]

#include <iomanip>
#include <sstream>

#include <mpi.h>

#include "mpiHelpers.hpp"

void showPhi(size_t Lx, size_t Ly, double * phi) {
    std::stringstream s;

    s << std::fixed << std::setprecision(3);
    s << "process #" << procID << ":" << std::endl;
    for     (auto i = 0u; i<Ly; ++i) {
        for (auto j = 0u; j<Lx; ++j) {
            s << std::setw(5) << phi[j*Ly + i] << "  ";
        }
        s << std::endl;
    }
    s << std::endl;
    showAndClear(s);
}

void task01_field() {
    const int Lx = Nprocs << 2;                                                 // make it a "nice problem" that can be divided evenly
    const int Ly = 10;

    const int Lx_local = Lx / Nprocs + 2;

    double * phi = new double[Lx_local * Ly];

    MPI_Datatype column_t;
    MPI_Type_contiguous(Ly, MPI_DOUBLE, &column_t);
    MPI_Type_commit(&column_t);

    // init local fields with dummy data
    for (size_t n = 0; n < Lx_local * Ly; ++n) {phi[n] = procID;}

    // whom to send data to
    const int neighbour_right = (procID +          1) % Nprocs;
    const int neighbour_left  = (procID + Nprocs - 1) % Nprocs;

    const int tag_left  = 0;
    const int tag_right = 1;

    // receive buffers
    double * data_left  = new double[Ly];
    double * data_right = new double[Ly];

    // send left boundary
    MPI_Request req_to_left;
    MPI_Isend(phi + Ly, 1, column_t,
              neighbour_left, tag_left, MPI::COMM_WORLD, &req_to_left);

    // send right boundary
    MPI_Request req_to_right;
    MPI_Isend(phi + (Lx_local - 2) * Ly, 1, column_t,
              neighbour_right, tag_right, MPI::COMM_WORLD, &req_to_right);

    // get left boundary
    MPI_Recv(data_right, 1, column_t,
             neighbour_right, tag_left, MPI::COMM_WORLD, MPI_STATUS_IGNORE);

    // get right boundary
    MPI_Recv(data_left, 1, column_t,
             neighbour_left, tag_right, MPI::COMM_WORLD, MPI_STATUS_IGNORE);

    // copy back into useData
    for (size_t i = 0; i < Ly; ++i) {
        phi[                      i] = data_left [i];
        phi[(Lx_local - 1) * Ly + i] = data_right[i];
    }

    showPhi(Lx_local, Ly, phi);

    delete[] phi;
    delete[] data_left;
    delete[] data_right;
}
