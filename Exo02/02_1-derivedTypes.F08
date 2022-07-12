function int_matrix_resize (mat, newRows, newCols) result(allocation_status)
    implicit none

    integer, allocatable, intent(inout) :: mat(:,:)
    integer             , intent(in)    :: newRows, newCols

    integer                             :: oldRows, oldCols

    integer, allocatable                :: temp_array(:,:)
    integer                             :: allocation_status

    oldRows = ubound(mat, 1)
    oldCols = ubound(mat, 2)

    if (oldRows > newRows) oldRows = newRows
    if (oldCols > newCols) oldCols = newCols

    print *, oldRows, oldCols

    allocate( temp_array(1:newRows,1:newCols), stat=allocation_status )
    if (allocation_status /= 0) then
       print *, "Failed to allocate memory in resize process"
       return
    endif

    temp_array(:oldRows, :oldCols) = mat(:oldRows, :oldCols)

    call move_alloc(temp_array, mat)
end function int_matrix_resize

! ============================================================================ !

program derivedTypes
    implicit none

    type myfield
        integer, allocatable    :: elem(:,:)
        logical                 :: initialised = .false.
    end type myfield

    type(myfield)           :: phi
    integer, allocatable    :: temp_array(:,:)
    integer                 :: allocation_status

    integer                 :: i, j
    integer                 :: rows  = 10, cols  = 3
    integer                 :: rows2 = 10, cols2 = 3

    interface
        function int_matrix_resize(mat, newRows, newCols) result(allocation_status)
            integer, allocatable, intent(inout) :: mat(:,:)
            integer             , intent(in)    :: newRows, newCols
            integer                             :: allocation_status
        end function int_matrix_resize
    end interface

    ! ------------------------------------------------------------------------ !

    allocate( phi%elem(1:rows,1:cols), stat=allocation_status )
    phi%initialised = (allocation_status == 0)
    if (.not. phi%initialised) then
        print *, "Failed to allocate memory"
        stop 1
    endif

    forall (i=1:rows, j=1:cols)
        phi%elem(i,j) = 20*(j-1) + 2*i
    end forall

    print *, "phi elements"
    print "(3(i2, 2x))", ((phi%elem(i,j), j=1,cols), i=1,rows)

    ! ======================================================================== !
    ! non-generalized version of int_matrix_resize

!     rows = 11
!     cols = 4
!
!     allocate( temp_array(1:rows,1:cols), stat=allocation_status )
!     if (allocation_status /= 0) then
!         print *, "Failed to allocate memory in resize process"
!         deallocate(phi%elem)
!         stop 1
!     else
!         print *, "Temp space for resize allocated"
!     endif
!
!     temp_array = phi%elem
!     call move_alloc(temp_array, phi%elem)

    ! ------------------------------------------------------------------------ !
    ! using subroutine

    rows = 11
    cols = 4
    allocation_status = int_matrix_resize(phi%elem, rows, cols)

    phi%elem(rows, :) = [ (2*i + 1, i=1,cols) ]
    phi%elem(:, cols) = [ (2*i + 1, i=1,rows) ]

    print *, "phi elements, resized"
    print "(4(i2, 2x))", ((phi%elem(i,j), j=1,cols), i=1,rows)

    rows = 10
    cols = 3
    allocation_status = int_matrix_resize(phi%elem, rows, cols)

    print *, "phi elements, size restored"
    print "(3(i2, 2x))", ((phi%elem(i,j), j=1,cols), i=1,rows)

    ! ------------------------------------------------------------------------ !
    ! not using move_alloc

    allocate( temp_array(1:rows,1:cols), stat=allocation_status )
    if (allocation_status /= 0) then
        print *, "Failed to allocate memory in resize process"
        deallocate(phi%elem)
        stop 1
    endif

    temp_array = phi%elem

    rows = 11
    cols = 4

    deallocate(phi%elem)
    allocate( phi%elem(1:rows,1:cols), stat=allocation_status )
    phi%initialised = (allocation_status == 0)
    if (.not. phi%initialised) then
        print *, "Failed to reallocate memory"
        stop 1
    endif

    phi%elem(:rows2, :cols2) = temp_array(:rows2, :cols2)

    phi%elem(rows, :) = [ (2*i + 1, i=1,cols) ]
    phi%elem(:, cols) = [ (2*i + 1, i=1,rows) ]

    deallocate(temp_array)

    print *, "phi elements, resized"
    print "(4(i2, 2x))", ((phi%elem(i,j), j=1,cols), i=1,rows)

    ! ------------------------------------------------------------------------ !

    if ( allocated(phi%elem) ) then
        deallocate(phi%elem)
    endif
end program derivedTypes
