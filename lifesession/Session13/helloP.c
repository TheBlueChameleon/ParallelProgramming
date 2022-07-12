#include <mpi.h>
#include <stdio.h>

int main (int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int me, Nprocs, length;
    char pName[MPI_MAX_PROCESSOR_NAME];
    
    MPI_Comm_rank         (MPI_COMM_WORLD, &me);
    MPI_Comm_size         (MPI_COMM_WORLD, &Nprocs);
    MPI_Get_processor_name(pName ,&length);
    
    int errCode = 38759;
    //int err = MPI_Abort(MPI_COMM_WORLD, errCode);
    
    printf("hello from proc %d/%d on machine '%s'\n", me, Nprocs, pName);
    
    MPI_Finalize();
    
    //printf("Post finalize: error was: %d\n", err);  // is never invoked.
}
