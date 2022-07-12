program test
    use omp_lib

    integer :: nArgs
    character(len=80) :: clArg = "undef"

    nArgs = command_argument_count()

    if (nArgs > 0) call get_command_argument(1, clArg )

    !$omp parallel
    print *, "Thread ", omp_get_thread_num(), ":", clArg, "/", nArgs
    !$omp end parallel
end program test
