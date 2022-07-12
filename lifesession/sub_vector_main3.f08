program example_sub
    implicit none

    real    :: array(1:10)
    integer :: i

    array = [(i*2, i=1,10)]

    call sub1(array)

contains

    subroutine sub1(i)
        implicit none
        real:: i(:)

        print *, "In sub1", i
    end subroutine sub1
end program example_sub
