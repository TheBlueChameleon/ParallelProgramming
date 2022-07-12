program enlarge_matrix

implicit none
    real, allocatable :: A(:,:), tmp(:,:)

    allocate(A(20,20)); A = 1.23

    print *, A

    ! Increase size of A to (100,100)
    allocate(tmp(100,100))
    tmp(1:20, 1:20) = A
    call move_alloc(tmp, A)             ! this also deallocates the "old" A

    deallocate(A)
end program enlarge_matrix
