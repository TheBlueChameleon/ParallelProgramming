program roots
    implicit none

    complex(8) :: a, b, c
    complex(8) :: discriminant
    complex(8) :: x, y

    print *, "Give the values for a,b and c in the form (x, y) where x is the real part and y is the imaginary part!"
    read *, a,b,c

    discriminant = b**2 - 4*a*c

    if (discriminant == 0) then
        print *, "only one solution exists: ", -b / (2*a)
    else
        x = -b + sqrt(discriminant) / (2*a)
        y = -b - sqrt(discriminant) / (2*a)

        print *, "two solutions exist:"
        print *, x
        print *, y
    endif
end program roots
