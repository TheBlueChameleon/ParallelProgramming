function fib(n) result(reVal)
    implicit none
    integer, intent(in) :: n
    integer :: i, x, y, z
    integer :: reVal

    x = 0
    y = 1
    z = 0

    do i=1, n
        z = x + y
        x = y
        y = z
    end do

    reVal = z
end function


program fibonacci
    implicit none
    integer :: n
    integer :: upper
    integer :: f_val

    integer :: fib

    print *, "Specify upper limit for the Fibonacci sequence:"
    read *, upper

    do n = 1, upper
        f_val = fib(n)
        print *, n, f_val
        print *
    end do

end program fibonacci
