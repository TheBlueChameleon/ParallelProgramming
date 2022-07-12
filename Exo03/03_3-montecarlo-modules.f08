module mc_funcs
    implicit none
    real   , parameter :: pi = 4. * atan(1.)

contains
    function mcarlo_nsphere(n, opt_accuracy) result(V)

        integer, intent(in)             :: n
        integer, optional, intent(in)   :: opt_accuracy
        real                            :: V

        integer                         :: i, j, dots
        integer                         :: accuracy
        real, allocatable               :: x(:), l

        if (present(opt_accuracy)) then
            if (opt_accuracy < 1) then
                V = -1
                return
            end if
            accuracy = opt_accuracy
        else
            accuracy = 10000
        end if

        dots = 0
        allocate(x(n))

        ! index i: number of dimensions
        ! index j: runs over random points

        do j=1, accuracy
            do i=1, n
                call random_number(x(i))
                ! random_number can do a vector in one go
            end do
            l = sum(x**2)

            dots = dots + merge(1, 0, l < 1)      ! l < 1 ? 1 : 0
        end do

        deallocate(x)

        V = real(dots) / accuracy
    end function

    ! ------------------------------------------------------------------------ !

    real function exact_nsphere(n)
        integer, intent(in) :: n
        integer             :: k

        if (n < 0) error stop 'volume is undefined for negative dimensions'

        if ( mod(n, 2) == 0 ) then          ! even case
            k = n / 2
            exact_nsphere = (pi**k) / factorial(k)

        else                                ! odd case
            k = (n-1) / 2
            exact_nsphere = (2**(k+1) * pi**k) / odd_factorial(n)
        end if
    end function

    ! ------------------------------------------------------------------------ !

    integer function factorial(n)
        integer, intent(in) :: n
        integer             :: i

        if (n < 0) error stop 'factorial is undefined for negative integers'

        factorial = product( [(i,i=1,n)] )
    end function

    ! ------------------------------------------------------------------------ !

    integer function odd_factorial(n)
        integer, intent(in) :: n
        integer             :: i

        if (n < 0) error stop 'odd factorial is undefined for negative integers'
        if (mod(n, 2) == 0) error stop 'odd factorial is undefined for even integers'

        odd_factorial = product( [((2*i + 1), i = 0, (n-1)/2)] )
    end function
end module

! ============================================================================ !

program monte_carlo
    use mc_funcs
    implicit none

    integer, parameter :: reruns = 7

    real    :: V_mc, V_xc, delta
    integer :: n, k, acc

    real :: total_error(2:reruns)

    do k=2, reruns
        total_error(k) = 0
        acc = 10**k

        print "('run ', i1, ' with ', i8, ' sampled dots')", k-1, acc
        print *

        print *, "dimension | MC-Volume | Exact Volume | Rel. Error"
        print *, "----------+-----------+--------------+-----------"
        do n = 1, 5
            V_mc = mcarlo_nsphere(n, acc)
            V_xc = exact_nsphere(n) / (2**n)
            delta = abs(V_mc - V_xc) / V_xc

            total_error(k) = total_error(k) + delta

            print "(2x, i1, 8x, '|', f9.6, 2x, '|', f9.6, 5x, '|', f9.6)", &
                n, V_mc, V_xc, delta
        end do
        print *
    end do

    print "(40('~'))"
    print *

    print *, "run | sampled points | total error"
    print *, "----+----------------+------------"
    do k=2, reruns
        print "(2x, i1, 2x, '|', 7x, i8, 1x, '|' f9.6)", k-1, 10**k, total_error(k)
    end do

    print *
    print "(40('~'))"
    print *

    print *, "This should give a 1/sqrt(N) trend"
end program
