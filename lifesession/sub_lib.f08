module subroutines
contains
    subroutine sub1(i)
        implicit none
        real :: i
        print *, "In sub1", i
    end subroutine sub1
end module
