module polynomial_mod
    implicit none

    type polynomail_t
        private
        real, allocatable, dimension(:) :: coeffs
        integer                         :: degree

    contains
        procedure, public   :: print
        final               :: DTor
    end type

    ! ........................................................................ !

    interface polynomail_t
        module procedure CTor
    end interface

    interface operator(+)
        module procedure add
    end interface

    interface operator(*)
        module procedure mul_real_right
        module procedure mul_real_left
    end interface

    ! ........................................................................ !

    private
    public :: polynomail_t
    public :: operator(+), operator(*)
    public :: derivative

! ............................................................................ !
contains
! ............................................................................ !

    function CTor(coeffs) result(this)
        real, intent(in) :: coeffs(:)
        type(polynomail_t) :: this

!         print *, "CTor", coeffs

        this%degree = ubound(coeffs, 1)
        this%coeffs = coeffs
    end function

    subroutine DTor(this)
        type(polynomail_t), intent(inout) :: this

!         print *, "DTor", this%coeffs

        if (allocated(this%coeffs)) deallocate(this%coeffs)
    end subroutine

    ! ........................................................................ !

    subroutine print(this)
        class(polynomail_t), intent(in)  :: this

        integer :: i

        do i = this%degree, 1, -1
            write (*, "(f7.3, 'x^', i1.1)", advance='no') this%coeffs(i), i-1
        end do
        print *
    end subroutine

    ! ........................................................................ !

    function optionalNull (array, i) result(x)
        real,    intent(in) :: array(:)
        integer, intent(in) :: i
        real                :: x

        if (i > ubound(array, 1)) then
            x = 0.
        else
            x = array(i)
        end if
    end function

    function add (lhs, rhs) result(this)
        type(polynomail_t), intent(in)  :: lhs, rhs
        type(polynomail_t)              :: this

        integer :: degree
        integer :: i

        degree = max( lhs%degree, rhs%degree )
        this%degree = degree
        allocate(this%coeffs(degree))

        do i=1, degree
            this%coeffs(i) = optionalNull(lhs%coeffs, i) + &
                             optionalNull(rhs%coeffs, i)
        end do
    end function

    ! ........................................................................ !

    function mul_real_right (lhs, rhs) result(this)
        type(polynomail_t), intent(in)  :: lhs
        real              , intent(in)  :: rhs
        type(polynomail_t)              :: this

        this%degree = lhs%degree
        allocate(this%coeffs(this%degree))

        this%coeffs = lhs%coeffs * rhs
    end function

    function mul_real_left (lhs, rhs) result(this)
        real              , intent(in)  :: lhs
        type(polynomail_t), intent(in)  :: rhs
        type(polynomail_t)              :: this

        this = mul_real_right(rhs, lhs)
    end function

    ! ........................................................................ !

    function derivative(self) result(this)
        type(polynomail_t), intent(in)  :: self
        type(polynomail_t)              :: this

        integer :: i

        this%degree = self%degree - 1
        this%coeffs = [(i * self%coeffs(i + 1), i=1, this%degree)]
    end function

end module

! ============================================================================ !

subroutine unittest
    use polynomial_mod
    implicit none

    type(polynomail_t) :: p, q, r, d

    p = polynomail_t([0., 1.])
    q = polynomail_t([1., 2., 3.])

    r = 3. * (p + q) * 2.
    d = derivative(r)

    call r%print
    call d%print
end subroutine

! ............................................................................ !

program polynomial_prg
    call unittest
end program
