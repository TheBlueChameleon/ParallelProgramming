program mask
    implicit none
    integer :: array(3) = [1,2,3]
    logical :: foobar(3)

    foobar(:) = array > 1

    print *, foobar
end program mask
