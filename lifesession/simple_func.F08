program example_func
    implicit none
    integer :: j = 4
    integer :: func, foo, bar

    print *, func(j)
    print *, foo (j)
    print *, bar (j)
end program example_func

integer function func(j)
    implicit none
    integer :: j
    func = 2*j
end function func

function foo(arg)
    implicit none
    integer, intent(in) :: arg
    integer             :: foo

    foo = arg ** 2
end function

function bar(arg) result(reVal)
    implicit none
    integer, intent(in) :: arg
    integer             :: reVal

    reVal = arg / 2
end function
