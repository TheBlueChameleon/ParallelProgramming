program example_sub
    implicit none

    interface
        subroutine sub1(i)
            implicit none
            real:: i(:)
        end subroutine
    end interface

    real    :: array(1:10)
    integer :: i

    array = [(i*2, i=1,10)]

    call sub1(array)
end program example_sub
