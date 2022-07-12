program loop
    implicit none
    integer :: i, j

    do j=1,3
        i = 1
        do
            i=i+1
            if (i== 5) cycle
            print *,i
            if (i==10) exit
        end do
    end do
end program loop
