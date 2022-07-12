program example_sub
    implicit none

    integer :: i = 1

    interface
        subroutine sub1(i)
            real :: i
        end subroutine
    end interface

    call sub1(i)
end program example_sub
