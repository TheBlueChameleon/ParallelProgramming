program avg_and_var
    implicit none

    integer, parameter  :: N = 1000
    integer             :: i

    real :: v(N)
    real :: average, variance

    call random_number(v)

    average = sum(v) / size(v)

    do i=1,N
        variance = variance + (v(i) - average)**2
    end do
    variance = sqrt(variance / N)

    ! var = sqrt( (sum(v**2) - sum(v)**2) / N)

    print *, "Average :", average
    print *, "Variance:", variance
end program avg_and_var
