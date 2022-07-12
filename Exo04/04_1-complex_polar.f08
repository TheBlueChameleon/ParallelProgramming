module Complex_Polar
    implicit none

    type Complex_t
        private
        real :: rho
        real :: phi
    end type

    real, parameter :: pi = 4. * atan(1.)
    character(len=29), parameter :: fmt_complex_polar = "(f8.3, ' * exp(', f6.3, 'i)')"

    interface Complex_t
        module procedure get_complex_polar_from_cartesian
    end interface

    interface operator(+)
        module procedure add_complex_polar
    end interface

    interface operator(-)
        module procedure sub_complex_polar
    end interface

    interface operator(*)
        module procedure mul_complex_polar
    end interface

    interface operator(/)
        module procedure div_complex_polar
    end interface

    interface operator(**)
        module procedure pow_complex_polar
    end interface

    private
    public :: Complex_t, get_real, get_imag, get_modulus, get_argument
    public :: operator(+), operator(-), operator(*), operator(/), operator(**)
    public :: fmt_complex_polar, pi

contains
    ! ======================================================================== !
    ! pseudo-constructor

    function get_complex_polar_from_cartesian(x, y) result(z)
        real, intent(in)    :: x, y
        type(Complex_t)     :: z

        z%rho = sqrt(x**2 + y**2)
        z%phi = atan2(y, x)
    end function

    function get_complex_polar_from_intrinsic(z0) result(z)
        complex, intent(in) :: z0
        type(Complex_t)     :: z

        z = get_complex_polar_from_cartesian( realpart(z0), imagpart(z0) )
    end function

    ! ======================================================================== !
    ! analyzer

    function get_real(z) result(x)
        type(Complex_t), intent(in) :: z
        real                        :: x

         x = z%rho * cos(z%phi)
    end function

    function get_imag(z) result(x)
        type(Complex_t), intent(in) :: z
        real                        :: x

         x = z%rho * sin(z%phi)
    end function

    function get_modulus(z) result(x)
        type(Complex_t), intent(in) :: z
        real                        :: x

         x = z%rho
    end function

    function get_argument(z) result(x)
        type(Complex_t), intent(in) :: z
        real                        :: x

         x = z%phi
    end function

    ! ======================================================================== !
    ! arithmetic

    function add_complex_polar(z1, z2) result(z)
        type(Complex_t), intent(in) :: z1, z2
        type(Complex_t)             :: z
        real                        :: x, y

        x = get_real(z1) + get_real(z2)
        y = get_imag(z1) + get_imag(z2)

        z = get_complex_polar_from_cartesian(x, y)
    end function

    function sub_complex_polar(z1, z2) result(z)
        type(Complex_t), intent(in) :: z1, z2
        type(Complex_t)             :: z
        real                        :: x, y

        x = get_real(z1) - get_real(z2)
        y = get_imag(z1) - get_imag(z2)

        z = get_complex_polar_from_cartesian(x, y)
    end function

    function mul_complex_polar(z1, z2) result(z)
        type(Complex_t), intent(in) :: z1, z2
        complex                     :: za, zb, z0
        type(Complex_t)             :: z

        za = complex( get_real(z1), get_imag(z1) )
        zb = complex( get_real(z2), get_imag(z2) )

        z0 = za * zb

        z = get_complex_polar_from_intrinsic(z0)
    end function

    function div_complex_polar(z1, z2) result(z)
        type(Complex_t), intent(in) :: z1, z2
        complex                     :: za, zb, z0
        type(Complex_t)             :: z

        za = complex( get_real(z1), get_imag(z1) )
        zb = complex( get_real(z2), get_imag(z2) )

        z0 = za / zb

        z = get_complex_polar_from_intrinsic(z0)
    end function

    function pow_complex_polar(z1, z2) result(z)
        type(Complex_t), intent(in) :: z1, z2
        complex                     :: za, zb, z0
        type(Complex_t)             :: z

        za = complex( get_real(z1), get_imag(z1) )
        zb = complex( get_real(z2), get_imag(z2) )

        z0 = za ** zb

        z = get_complex_polar_from_intrinsic(z0)
    end function
end module

program complex_polar_main
    use Complex_Polar
    implicit none

    type(Complex_t) :: z1, z2, z3

    z1 = Complex_t(1., 0.)                                                      ! 1
    z2 = Complex_t(1., pi / 2)                                                  ! 1i

    call show_components(z1)
    call show_components(z2)

    print *, "ADDITION"
    z3 = (z1 + z2)
    call show_components(z3)

    print *, "SUBTRACTION"
    z3 = (z1 + z2)
    call show_components(z3)

    print *, "MULTIPLICATION"
    z3 = (z1 * z2)
    call show_components(z3)

    print *, "DIVISION"
    z3 = (z1 / z2)
    call show_components(z3)

    print *, "EXPONENTIATION"
    z3 = (z2 ** z2)                                                             ! i^i ~=~ 0.20787957635
    call show_components(z3)
contains

    subroutine show_components(z)
        type(Complex_t), intent(in) :: z

        print *, "components of complex number:"
        !print fmt_complex_polar, z
        print fmt_complex_polar, get_real(z), get_imag(z)
        print "('    real part     : ', f7.3)", get_real    (z)
        print "('    imaginary part: ', f7.3)", get_imag    (z)
        print "('    modulus       : ', f7.3)", get_modulus (z)
        print "('    argument      : ', f7.3)", get_argument(z)
        print *
    end subroutine
end program
