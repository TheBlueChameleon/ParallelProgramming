program pointers
    implicit none

    real, dimension(10,10), target :: a,b,c
    real, dimension(:,:), pointer :: aa,bb,cc

    integer :: j,k

    a = 1
    b = 2
    c = 3

    do j=2,10
    do k=1,9
        a(j:k+1, j-1:k) = a(j:k+1, j-1:k) &
                        + a(j:k+1, j-1:k)

        a(j:k+1, j-1:k) = b(j:k+1, j-1:k) &
                        + c(j:k+1, j-1:k) &
                        + c(j:k+1, j:k+1)
    end do
    end do

    print *, "non simplified"
    print "(10(f10.0, 1x))", ((a(j,k), k=1,10), j=1,10)
    print *

    a = 1
    b = 2
    c = 3

    do j=2,10
    do k=1,9
        aa => a(j:k+1, j-1:k)
        bb => b(j:k+1, j-1:k)
        cc => c(j:k+1, j-1:k)

        aa = aa + aa
        aa = bb + cc + c(j:k+1, j:k+1)
        ! cc(:, )
    end do
    end do

    print *, "simplified"
    print "(10(f10.0, 1x))", ((a(j,k), k=1,10), j=1,10)
    print *
end program pointers
