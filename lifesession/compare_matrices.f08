program intrinsic
    implicit none

    real(8) :: A(5,5)
    real :: B(5,5)

    call random_number(A)
    B=A

    ! test if all elements of B are equal to those of A
    print *, "naive check: copying done correctly: ", all(B == A)
    print *, "epsilon type check                 : ", all(abs(B - A) < 10 * epsilon(B))
end program intrinsic
