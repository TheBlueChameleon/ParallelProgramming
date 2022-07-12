program complex_polar_main
    use Complex_Polar
    implicit none

    type(Complex_t) ::  z1 = Complex_t(1., 0.)    , &                           ! 1
                        z2 = Complex_t(1., pi / 2), &                           ! 1i
                        z3

    call show_components(z1)
    call show_components(z2)

    print *, "ADDITION"
    z3 = add_complex_polar(z1, z2)
    call show_components(z3)

    print *, "SUBTRACTION"
    z3 = sub_complex_polar(z1, z2)
    call show_components(z3)

    print *, "MULTIPLICATION"
    z3 = mul_complex_polar(z1, z2)
    call show_components(z3)

    print *, "DIVISION"
    z3 = div_complex_polar(z1, z2)
    call show_components(z3)

    print *, "EXPONENTIATION"
    z3 = pow_complex_polar(z2, z2)                                              ! i^i ~=~ 0.20787957635
    call show_components(z3)
contains

    subroutine show_components(z)
        type(Complex_t), intent(in) :: z

        print *, "components of complex number:"
        print fmt_complex_polar, z
        print "('    real part     : ', f7.3)", get_real    (z)
        print "('    imaginary part: ', f7.3)", get_imag    (z)
        print "('    modulus       : ', f7.3)", get_modulus (z)
        print "('    argument      : ', f7.3)", get_argument(z)
        print *
    end subroutine
end program
