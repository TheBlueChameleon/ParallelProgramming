program concurrent
    implicit none

    integer, parameter  :: N = 10**9
    integer             :: i
    real                :: v(N)
    real(8)             :: t0, t1, t2
    real(8)             :: runtime_concurrent, runtime_linear

    v=1.0

    call cpu_time(t0)

    do concurrent (i=1:N)
        v(i)=v(i)*2
    end do
    call cpu_time(t1)

    do i=1,N
        v(i)=v(i)*2
    end do
    call cpu_time(t2)

    runtime_concurrent  = t1 - t0
    runtime_linear      = t2 - t1

    print *, "CONCURRENT: ", runtime_concurrent
    print *, "LINEAR    : ", runtime_linear
    print *, "DELTA     : ", runtime_linear - runtime_concurrent
end program concurrent
