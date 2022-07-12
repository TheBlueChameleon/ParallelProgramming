program array_intrinsics
    implicit none

    integer, parameter  :: ik = selected_int_kind(12)

    integer, parameter  :: N = 10_ik ** 9
    integer             :: i

    real                :: v(N)
    real(8)             :: t0, t1, t2, t3
    real(8)             :: runtime_concurrent, runtime_linear, runtime_forall

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

    forall (i=1:N)
        v(i)=v(i)*2
    end forall

    call cpu_time(t3)

    runtime_concurrent  = t1 - t0
    runtime_linear      = t2 - t1
    runtime_forall      = t3 - t2

    print *, "CONCURRENT: ", runtime_concurrent
    print *, "LINEAR    : ", runtime_linear
    print *, "FORALL    : ", runtime_forall
end program array_intrinsics

