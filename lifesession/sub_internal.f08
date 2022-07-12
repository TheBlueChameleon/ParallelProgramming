program example_sub
    implicit none

    integer :: i = 1
    real    :: r = 1.

    call sub1(r)
    call sub1(r, i)
    call sub1(i=2, r=2.)

contains

    subroutine sub1(r,i)
        implicit none
        integer, optional   :: i
        real                :: r

        if (present(i)) then
            print *, "opt. arg present. r=", r, "i=", i
        else
            print *, "no opt arg. r=", r
            ! print *, i        ! "works", prints zero, but ...
            ! i = 2             ! ... write access gives a segfault
        end if
    end subroutine sub1
end program example_sub
