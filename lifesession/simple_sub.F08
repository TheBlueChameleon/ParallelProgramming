program example_sub
    implicit none
    integer :: arg = 99

    call sub1(arg)
    print "(a15, 1x, i3)", "In main:", arg
end program example_sub

subroutine sub1(arg)
    implicit none
    integer, intent(inout) :: arg

    print "(a15, 1x, i3)", "In sub1:", arg

    arg = -99
end subroutine sub1
