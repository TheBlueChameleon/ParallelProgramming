program allocate
    implicit none
    ! variable declarations

    integer, parameter      :: ik = selected_int_kind(12)

    integer(8)              :: i
    integer(8), parameter   :: n = 10_ik ** 5

    real, allocatable       :: v(:)
    integer                 :: allocation_status

    ! allocate the vector v to be of length n
    allocate( v(n), stat=allocation_status )

    ! failsafe
    if (allocation_status /= 0) then
        print *, "could not allocate enough memory"
        stop 1
    endif

    if (.not. allocated(v) ) then
        print *, "allocation of v assertation failed"
        stop 1
    end if

    ! assignment

    do i= 1,n
        v(i) = 1.0*i
    end do

    ! deallocate v
    deallocate(v)

    if (allocated(v) ) then
        print *, "free of v assertation failed"
        stop 1
    end if

    ! stuff?
    do i=1,n
        if(mod(i,10000)==0) print *,i
    end do
end program allocate
