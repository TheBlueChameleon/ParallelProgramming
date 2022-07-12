module Array_mod
    interface append_to_array
        module procedure append_to_real_array
        module procedure append_to_int_array
        module procedure append_to_dummy
        module procedure append_to_repeater
    end interface

    interface show_matrix
        module procedure showMatrix_real
    end interface

    private
    public populate_random, append_to_array, show_matrix

! ............................................................................ !
contains
! ............................................................................ !

    subroutine append_to_real_array(modify, appendee)
        real, allocatable, dimension(:), intent(inout)  :: modify
        real,              dimension(:), intent(in)     :: appendee

        real, allocatable, dimension(:) :: temp
        integer :: N, N0, i

        N0 = ubound(modify, 1)
        N  = N0 + ubound(appendee, 1)
        allocate(temp(N))

        temp(:N0)     = modify(:)
        temp(N0 + 1:) = appendee(:)

        call move_alloc(temp, modify)
    end subroutine

    ! ........................................................................ !

    subroutine append_to_int_array(modify, appendee)
        integer, allocatable, dimension(:), intent(inout)  :: modify
        integer,              dimension(:), intent(in)     :: appendee

        integer, allocatable, dimension(:) :: temp
        integer :: N, N0, i

        N0 = ubound(modify, 1)
        N  = N0 + ubound(appendee, 1)
        allocate(temp(N))

        temp(:N0)     = modify(:)
        temp(N0 + 1:) = appendee(:)

        call move_alloc(temp, modify)
    end subroutine

    ! ........................................................................ !

    subroutine append_to_dummy ()
        print *, "dummy version"
    end subroutine

    ! ........................................................................ !

    subroutine append_to_repeater (array)
        real, allocatable, dimension(:), intent(in)  :: array

        print *, "dummy version on", array
    end subroutine

    ! ------------------------------------------------------------------------ !

    subroutine showMatrix_real(matrix, optNumFmt)
        real, dimension(:,:), intent(in)    :: matrix
        character, optional                 :: optNumFmt

        character(len=80)       :: fmt
        character(len=80)       :: numFmt
        integer, dimension(2)   :: shp

        shp = shape(matrix)

        if (present(optNumFmt)) then
            numFmt = optNumFmt
        else
            numFmt = "f5.1, 1x"
        end if

        write(fmt, fmt="('(', i2, '(', a20, ') )')") shp(2), numFmt

        print fmt, matrix
    end subroutine

    ! ------------------------------------------------------------------------ !

    subroutine populate_random (matrix, percentage)
        real, dimension(:,:), intent(inout) :: matrix
        real                , intent(in)    :: percentage

        integer :: rows, cols
        integer :: r, c
        real    :: x

        rows = ubound(matrix, 1)
        cols = ubound(matrix, 1)

        do      c = 1, cols
            do  r = 1, rows
                call random_number(x)
                if (x < percentage) call random_number(matrix(r, c))
            end do
        end do
    end subroutine
end module

! ============================================================================ !

module SparseMatrix_mod
    implicit none

    type SparseMatrix_t
        private
        real   , allocatable, dimension(:) :: val
        integer, allocatable, dimension(:) :: row, col

    contains
        final :: DTor
    end type

    ! ........................................................................ !

    interface SparseMatrix_t
        module procedure CTor
    end interface

    ! ........................................................................ !

    private
    public :: SparseMatrix_t
    public :: mul_sparseMatrix_with_vector

! ............................................................................ !
contains
! ............................................................................ !

    function CTor(matrix) result(this)
        use Array_mod

        real, dimension(:,:), intent(in) :: matrix
        type(SparseMatrix_t)             :: this

        integer :: rows, cols
        integer :: r, c

        logical :: newRow = .true.

        rows = ubound(matrix, 1)
        cols = ubound(matrix, 1)


        allocate( this%val(0) )
        allocate( this%row(0) )
        allocate( this%col(0) )

        do     r = 1, rows
            do c = 1, cols
                if (matrix(r, c) /= 0) then
                    call append_to_array(this%val, [matrix(r, c)])
                    call append_to_array(this%col, [c])

                    if (newRow) then
                        newRow = .false.
                        call append_to_array(this%row, [ubound(this%col, 1)])
                    end if
                    ! append to this%row if new row was started
                end if
            end do
            newRow = .true.
        end do

        call append_to_array(this%row, [ubound(this%col, 1) + 1])
    end function

    ! ........................................................................ !

    subroutine DTor(this)
        type(SparseMatrix_t), intent(inout) :: this

        if (allocated(this%val)) deallocate(this%val)
        if (allocated(this%row)) deallocate(this%row)
        if (allocated(this%col)) deallocate(this%col)
    end subroutine

    ! ------------------------------------------------------------------------ !

    function mul_sparseMatrix_with_vector (lhs, rhs) result(this)
        type(SparseMatrix_t), intent(in) :: lhs
        real, dimension(:)  , intent(in) :: rhs

        real, dimension(:), allocatable  :: this

        integer :: i, j, N, N0

        N0 = ubound(lhs%val, 1)
        N  = ubound(lhs%row, 1) - 1

        allocate( this(N + 1) )

        this = 0

        do i = 1, N
            do j = lhs%row(i) , lhs%row(i + 1) - 1
                this(i) = this(i) + lhs%val(j) * rhs( lhs%col(j) )
            end do
        end do


    end function
end module

! ============================================================================ !

subroutine unittest_array
    use Array_mod
    integer , allocatable, dimension(:) :: a, b
    real    , allocatable, dimension(:) :: x, y
    real    , dimension(2, 2)           :: M

    integer :: c = 1

    x = [0., 1., 2.]
    y = [-1.]
    call append_to_array(x, y)
    if (.not. all(x == [0., 1., 2., -1.])) stop "error concatenating real arrays"
    print *, "okay: real, fully allocated arrays"
    print *

    allocate( a(0) )
    b = [c]
    call append_to_array(a, b)
    if (.not. all(a == [1])) stop "error concatenating integer arrays"
    print *, "okay: integer, pseudo-allocated arrays"
    print *

    M = reshape([1,2,3,4], shape(M))
    call show_matrix(M)
    print *, "okay: show arbitrary sized matrix"
    print *

    M = 0
    call populate_random(M, 0.5)
    call show_matrix(M)
    print *, "okay: generate random matrix"

    print *, "Passed Unittest Array Functions"
    print *
end subroutine

! ............................................................................ !

subroutine unittest_sparseMatrix
    use SparseMatrix_mod
    implicit none

    real, parameter         :: eps = 100 * epsilon(eps)
    integer, parameter      :: N = 6

    real, dimension(N, N)   :: M = 0
    type(SparseMatrix_t)    :: SM

    real, dimension(N)      :: v, w, ws

    ! remember: Fortran is column major
    M = reshape((/ 10., 3., 0., 3.,  0.,  0., &
                    0., 9., 7., 0.,  8.,  4., &
                    0., 0., 8., 8.,  0.,  0., &
                    0., 0., 7., 7.,  9.,  0., &
                   -2., 0., 0., 5.,  9.,  2., &
                    0., 3., 0., 0., 13., -1.  &
                /), shape(M))
    SM = SparseMatrix_t(M)

    call random_number(v)

    w = matmul(M, v)
    ws = mul_sparseMatrix_with_vector(SM, v)

    print *, "Expected", w
    print *, "Got     ", ws

    if (.not. all(ws - w < eps)) stop "error in sparse matrix/vector multiplication (real)"
    print *, "okay: sparse matrix/vector multiplication (real)"

    print *, "Passed Unittest Sparse Matrix Functions"
    print *
end subroutine

subroutine walltimetest_sparseMatrix
    use SparseMatrix_mod
    use Array_mod
    implicit none

    integer, parameter                  :: N0 = 500, Nw = 1500, Nd = 500
    integer                             :: N

    real, allocatable, dimension(:, :)  :: M
    type(SparseMatrix_t)                :: SM

    real, allocatable, dimension(:)     :: v, w

    real, dimension(N0:Nw, 0:3)         :: times

    print *, "about to do runtime test. This may take some time..."

    do N = N0, Nw, Nd
        print *, " ... doing test for N = ", N, "..."

        ! take time for preparation, too
        call cpu_time( times(N, 0) )
            if (allocated(M)) deallocate(M)
            if (allocated(v)) deallocate(v)
            if (allocated(w)) deallocate(w)

            allocate(M(N, N))
            allocate(v(N))
            allocate(w(N))

            M  = 0
            call populate_random(M, 0.01)
            call random_number(v)
            SM = SparseMatrix_t(M)


        call cpu_time( times(N, 1) )
            w = matmul(M, v)
        call cpu_time( times(N, 2) )
            w = mul_sparseMatrix_with_vector(SM, v)
        call cpu_time( times(N, 3) )
    end do
    print *

    do N = N0, Nw, Nd
        print "('runtime for N = ', i5, ':')", N
        print "('    prep  :', f10.2, 1x, a3):", (times(N, 1) - times(N, 0)) * 1000000, "µs"
        print "('    full  :', f10.2, 1x, a3):", (times(N, 2) - times(N, 1)) * 1000000, "µs"
        print "('    sparse:', f10.2, 1x, a3):", (times(N, 3) - times(N, 2)) * 1000000, "µs"
    end do
end subroutine

! ............................................................................ !

program SparseMatrix_prg
    print *, "ENTRY"
    print *

    call unittest_array
    call unittest_sparseMatrix
    call walltimetest_sparseMatrix

    print *
    print *, "ALL DONE"
end program
