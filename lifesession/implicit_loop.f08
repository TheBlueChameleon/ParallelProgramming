program loop
    implicit none
    integer :: i,j

    print *, "i  : ", (i,i=1,4)
    print *, "i*j: ", ((i*j, i=1,4), j=1,4)
end program loop
