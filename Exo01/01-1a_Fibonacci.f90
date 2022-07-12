function fib(n) result(reVal)
    implicit none
    integer, intent(in) :: n
    integer :: i, x, y
    integer :: reVal

    x = 0
    y = 1
    z = 0

    reVal = 0

    do i=1, n
        x = y
        y = reVal
        reVal = x + y
    end do

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
    end do

end program fibonacci
