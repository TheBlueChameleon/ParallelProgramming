program autoAlloc
    integer, allocatable :: array(:)
    integer              :: static(10)

    array  = [(i, i=1,10)]      ! auto alloc
    static = [(i, i=1,10)]      ! no auto alloc on static

    array  = [(i, i=1,10)]      ! auto realloc

    deallocate(array)           ! no auto free
end program autoAlloc
