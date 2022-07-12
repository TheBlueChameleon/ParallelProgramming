program main
    use mpi
    integer :: error
    
    call MPI_Init(error)
    
    print *, "Hello"
    
    call MPI_Finalize(error)
end program main
