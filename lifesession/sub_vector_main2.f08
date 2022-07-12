program example_sub
    use subroutines
    implicit none

    real    :: array(1:10)
    integer :: i

    array = [(i*2, i=1,10)]

    call sub1(array)
end program example_sub
