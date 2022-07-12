program case
    implicit none

    integer :: i = 1
    character(len=10) :: arg

    if (command_argument_count() == 0) then
        print *, "Output stored in default file data.txt."
        call exit(0)
    end if

    do
        call get_command_argument(i,arg)

        select case (arg)
            case ('-h')
                print *, "Usage: test_case -f filename."
            case ('-f file1.txt')
                print *, "Output stored in file1.txt"
        end select

        print *, arg
        if (i == command_argument_count()) exit
        i=i+1
    end do
end program case
