#define PI 3.14159265358979323846100_kind_long_real

program precision
    implicit none

    ! ------------------------------------------------------------------------ !
    ! precision parameters

    integer, parameter :: kind_small_int = selected_int_kind( 2)
    integer, parameter :: kind_big_int   = selected_int_kind(20)

    integer, parameter :: kind_lores_real = selected_real_kind(5)
    integer, parameter :: kind_hires_real = selected_real_kind(14)
    integer, parameter :: kind_long_real  = selected_real_kind(18, 100)


    ! ------------------------------------------------------------------------ !
    ! true variables

    integer(kind=kind_small_int) :: i ! = 10000     ! i is of type integer(1), which cannot hold values greater than 127
    integer(kind=kind_big_int  ) :: j

    real(kind=kind_lores_real) :: r = PI
    real(kind=kind_hires_real) :: s = PI
    real(kind=kind_long_real ) :: t = PI

    real(kind=kind_lores_real) :: one5  = 1.0
    real(kind=kind_hires_real) :: one14 = 1.0
    real(kind=kind_long_real ) :: one18 = 1.0

    ! ------------------------------------------------------------------------ !
    ! main code

    print *, "VALUES OF PI"
    print "(a f25.23)", "LITERAL     ", PI
    print "(a f25.23)", "LONG DOUBLE ", t
    print "(a f25.23)", "DOUBLE      ", s
    print "(a f25.23)", "SINGLE      ", r

    print *, "---"
    print *, "PI FROM ARCTAN"
    print "(a f25.23)", "LITERAL     ", PI
    print "(a f25.23)", "LONG DOUBLE ", 4.* atan(one18)
    print "(a f25.23)", "DOUBLE      ", 4.* atan(one14)
    print "(a f25.23)", "SINGLE      ", 4.* atan(one5)


    print *, "---"
    print *, "EPSILON"
    print "(a f25.23 a f25.23 a f25.23)", "LONG DOUBLE ", t, " ", (t + 3*epsilon(t)), " ", (t + 0.9*epsilon(t))
    print "(a f25.23 a f25.23 a f25.23)", "DOUBLE      ", s, " ", (s + 3*epsilon(s)), " ", (s + 0.9*epsilon(s))
    print "(a f25.23 a f25.23 a f25.23)", "SINGLE      ", r, " ", (r + 3*epsilon(r)), " ", (r + 0.9*epsilon(r))
end program precision
