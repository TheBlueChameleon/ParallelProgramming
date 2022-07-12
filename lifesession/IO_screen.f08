program inout
    implicit none
    integer :: i=123456789
    real    :: r=12345.6789
    integer :: j,k

    print *, i, r
    write(*,'(i9,1x,f10.2)') i,r
    write(6,'(a,1x,i11.11,/,a,1x,e10.5)') "i= ",i,"r= ",r

    write(*,'(a)') "Input: k "
    read *, k
    print '(a2,tr10,i6)', "K ", k

    write(*, '(a)',advance="no") "Input: j "
    read(5, '(i3)') j
    print '(2(i10,1x))', k, j
end program inout
