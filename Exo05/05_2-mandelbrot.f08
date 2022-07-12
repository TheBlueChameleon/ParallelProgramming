module mandelbrot_mod
    implicit none

    private
    integer, parameter :: N_limit       = 50
    real   , parameter :: z_abs_limit   = 2.
    integer, parameter :: N_x = 2000, N_y = 1000

    public :: N_limit, N_x, N_y
    public :: get_x_coordinate_from_index, get_y_coordinate_from_index
    public :: point_in_set, writeList

! ............................................................................ !
contains
! ............................................................................ !

    real function get_x_coordinate_from_index(i)
        integer, intent(in) :: i
        get_x_coordinate_from_index = get_coordinate_from_index(i, N_x, -1.5)
    end function
    ! ........................................................................ !
    real function get_y_coordinate_from_index(i)
        integer, intent(in) :: i
        get_y_coordinate_from_index = get_coordinate_from_index(i, N_y, -0.5)
    end function
    ! ........................................................................ !
    real function get_coordinate_from_index(i, n, offset)
        integer, intent(in) :: i, n
        real, intent(in)    :: offset

        get_coordinate_from_index = (real(i) / n) + offset
    end function

    ! ------------------------------------------------------------------------ !

    logical function point_in_set(x, y)
        real, intent(in) :: x, y

        integer :: i
        complex :: z, c

        z = complex(x, y)
        c = z
        do i=1, N_limit
            z = z**2. + c
            if (abs(z) > z_abs_limit) then
                point_in_set = .false.
                return
            end if
        end do

        point_in_set = .true. !abs(z) < z_abs_limit
    end function

    ! ------------------------------------------------------------------------ !

    subroutine writeList (filename)
        character(len=*), intent(in)    :: filename

        integer                         :: id_unit
        integer                         :: i, j
        real                            :: x, y

        open(newunit = id_unit, file=filename, status='new')

        do      i = 1, N_x
            do  j = 1, N_y
                x = get_x_coordinate_from_index(i)
                y = get_y_coordinate_from_index(j)

                if ( point_in_set(x, y) ) then
                    write (id_unit, *) x, y
                end if

            end do
        end do
        close(id_unit)
    end subroutine
end module



program mandelbrot_prg
    use mandelbrot_mod

    implicit none

    call writeList("foo.bar")
end program
