program inquiry
    implicit none

    real :: A(5,5)
    integer :: i, j

    call random_number(A)

    print "(5(f10.8, 1x))", ((A(i,j), i=1,5), j=1,5)

    print *, ((A(i,j), i=1,5), j=1,5)
print *,A

    print *, "elements greater than 0.5:", count(A > .5)
    print *, "any less than 0.01       :", any(A < 0.01)
    print *, "biggest element          :", maxval(A)


end program inquiry
