module simple_mod
    implicit none
    private
    real, parameter, public :: pi = 3.14159265
    public                  :: area
    real, protected, public :: circum

    contains
        real function area(r)
            real, intent(in) :: r

            circum = r*2
            area = r**2 * pi
        end function
end module simple_mod

! ============================================================================ !

program test_mod
    use simple_mod !, only : area
    implicit none

    print *, area(1.0), pi, circum
    ! circum = 1
end program test_mod
