recursive function fib(n) result(reVal)
    implicit none
    integer, intent(in) :: n
    integer :: reVal

    if (n < 2) then
        reVal = n
        return
    end if

    reVal = fib(n - 1) + fib(n - 2)
end function


program fibonacci
    implicit none
    integer :: n
    integer :: upper
    integer :: fib

    print *, "Specify upper limit for the Fibonacci sequence:"
    read *, upper

    do n = 1, upper
        print *, n, fib(n)
    end do

end program fibonacci
