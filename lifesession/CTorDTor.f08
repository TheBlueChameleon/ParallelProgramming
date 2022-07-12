module position
    implicit none

    type point_t
        private
        real :: x, y

    contains
        procedure   :: print => print_point_t
        final       :: destroy
    end type point_t

    interface point_t
        module procedure start
    end interface point_t

    private
    public :: point_t

contains

    function start(x, y) result(p)
        real, intent(in) :: x, y
        type(point_t)    :: p

        print *, "CTor of point_t"

        p%x = x
        p%y = y
    end function

    subroutine destroy (this)
        type(point_t), intent(in) :: this

        print *, "DTor of point_t"
    end subroutine

    subroutine print_point_t (this)
        class(point_t), intent(in) :: this

        print "('('f7.3, ', ', f7.3')')", this%x, this%y
    end subroutine

end module position

subroutine calc_posn
    use position
    implicit none

    type(point_t) :: posn1

    posn1 = point_t(1.0, 2.0)
    ! print *, posn1%x      ! successfully made components of point_t private

    call posn1%print
end subroutine calc_posn

program test
    call calc_posn
end program test
