program Integration_main
    use Integration
    implicit none

    real(rk), parameter :: pi = 4. * atan(1.)
    real(rk), parameter :: sinsquared_zero_to_one = 0.2726756433_rk

    real(rk), external  :: const, sin2
    real(rk)            :: integral, delta
    integer(ik)         :: i, n

    print *, integrate              (const, 0., 1.5)
    print *, integrate_nonabstract  (const, 0., 1.5)
    print *, integrate_external     (const, 0., 1.5)

    print *, integrate(sin2 , 0., 2.*pi)

    do i = 1, 20
        n = 2**i
        integral = integrate(sin2, 0., 1., n)
        delta = abs(sinsquared_zero_to_one - integral)

        print *, n, integral, delta
    end do
end program

real(rk) function const (x)
    use Integration
    implicit none

    real(rk), intent(in) :: x

    const = 1.
end function

real(rk) function sin2 (x)
    use Integration
    implicit none

    real(rk), intent(in) :: x

    sin2 = sin(x)**2
end function
