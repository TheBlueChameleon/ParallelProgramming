module Integration
    implicit none

    public
    integer, parameter :: rk = selected_real_kind( 5)
    integer, parameter :: ik = selected_int_kind (10)

    abstract interface
        function IntKernel(x)
            import                  :: rk
            real(rk)                :: IntKernel
            real(rk), intent(in)    :: x
        end function IntKernel
    end interface

    ! ======================================================================== !
contains

    function integrate(func, a, b, intsteps) result(integral)
        ! interface
        procedure(IntKernel)                :: func
        real(rk), intent(in)                :: a, b
        integer(ik), optional, intent(in)   :: intsteps
        real(rk)                            :: integral

        ! local variables
        real(rk)                            :: x, dx
        integer(ik)                         :: i, n

        ! parameter setup
        if (present(intsteps)) then
            n = intsteps
        else
            n = 10000
        end if


        dx = (b-a) / n
        if ( epsilon(b) * n > (b-a)) then
            dx = epsilon(b)
            n = (b-a) / dx
        end if

        ! integration
        integral = 0.0_rk
        do i = 1, n
            x = a + (1.0_rk * i - 0.5_rk) * dx
            integral = integral + func(x)
        end do

        integral = integral * dx
    end function

    ! ------------------------------------------------------------------------ !

    function integrate_nonabstract(func, a, b, intsteps) result(integral)
        ! interface
        interface
            function func(x)
            import                  :: rk
            real(rk)                :: IntKernel
            real(rk), intent(in)    :: x
            end function
        end interface

        real(rk), intent(in)                :: a, b
        integer(ik), optional, intent(in)   :: intsteps
        real(rk)                            :: integral

        ! local variables
        real(rk)                            :: x, dx
        integer(ik)                         :: i, n

        ! parameter setup
        if (present(intsteps)) then
            n = intsteps
        else
            n = 10000
        end if

        dx = (b-a) / n
        if ( epsilon(b) * n > (b-a)) then
            dx = epsilon(b)
            n = (b-a) / dx
        end if

        ! integration
        integral = 0.0_rk
        do i = 1, n
            x = a + (1.0_rk * i - 0.5_rk) * dx
            integral = integral + func(x)
        end do

        integral = integral * dx
    end function

    ! ------------------------------------------------------------------------ !

    function integrate_external(func, a, b, intsteps) result(integral)
        ! interface
        real(rk), external                  :: func
        real(rk), intent(in)                :: a, b
        integer(ik), optional, intent(in)   :: intsteps
        real(rk)                            :: integral

        ! local variables
        real(rk)                            :: x, dx
        integer(ik)                         :: i, n

        ! parameter setup
        if (present(intsteps)) then
            n = intsteps
        else
            n = 10000
        end if

        dx = (b-a) / n
        if ( epsilon(b) * n > (b-a)) then
            dx = epsilon(b)
            n = (b-a) / dx
        end if

        ! integration
        integral = 0.0_rk
        do i = 1, n
            x = a + (1.0_rk * i - 0.5_rk) * dx
            integral = integral + func(x)
        end do

        integral = integral * dx
    end function
end module
