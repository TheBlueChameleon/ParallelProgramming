module position
    implicit none
    private

    integer, parameter :: rk = selected_real_kind(7)

    type point_t
        real(rk) :: x, y
    end type

    interface operator(+)
        module procedure add_points
    end interface

    interface print
        module procedure printOne
        module procedure printTwo
    end interface print

    public :: rk, point_t, operator(+), print

contains
    function add_points(lhs, rhs) result(r)
        type(point_t), intent(in)   :: lhs, rhs
        type(point_t)               :: r

        r%x = lhs%x + rhs%x
        r%y = lhs%y + rhs%y
    end function

    subroutine printOne(pnt)
        type(point_t), intent(in) :: pnt

        print "('(', f7.3,', ', f7.3,')')", pnt
    end subroutine

    subroutine printTwo(pnt1, pnt2)
        type(point_t), intent(in) :: pnt1, pnt2

        print "('(', f7.3,', ', f7.3,') and (', f7.3,', ', f7.3,')')", pnt1, pnt2
    end subroutine
end module position

program calc_posn
    use position
    implicit none

    type(point_t) :: posn1 = point_t(1.0, 2.0)
    type(point_t) :: posn2 = point_t(0.0, 2.0)

!     call printTwo(posn1, posn2)
!     call printOne(posn1 + posn2)

!     print *, add_points(posn1,posn2)
!     print *, posn1 + posn2

    call print(posn1 , posn2)
    call print(posn1 + posn2)
end program calc_posn
