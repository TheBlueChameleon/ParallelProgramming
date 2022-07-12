program test_io
    implicit none
    integer :: i, ios
    integer :: u
    real    :: x

    logical         :: ex
    character (15)  :: di, fo, ac, se
    integer         :: nu, st

    open(10,file='data.txt',iostat=ios, status='unknown')

    if (ios /= 0) then
        print *, "Error opening file 'data.txt': file already exists."
        print *, "Error code:", ios
        stop !"Error opening file 'data.txt': file already exists.", ios
    end if

    write(10,'(f10.5)') (i*.001,i=1,100)
    close(10)

    open(newunit=u, file='data.txt', iostat=ios, status='old')
    print *, "FILE ID:", u

    inquire(u,  exist = ex, direct = di, sequential = se, formatted = fo, &
                access = ac, number = nu, iostat=st)

    if (ios /= 0) then
        print *, "Error opening file 'data.txt'."
        print *, "Error code:", ios
        stop !"Error opening file 'data.txt': file already exists.", ios
    end if

    ! rewind u
    read (u, *, iostat=ios) x
    do while (ios == 0)
        print *, x
        read (u, *, iostat=ios) x
    end do

    if(st == 0) then ! Ausgabe, z.B.
        write (*,*) 'EXIST? ', ex ! EXIST? T
        write (*,*) 'DIRECT? ', di ! DIRECT? YES
        write (*,*) 'SEQUENTIAL? ', se ! SEQUENTIAL? YES
        write (*,*) 'FORMATTED? ', fo ! FORMATTED? YES
        write (*,*) 'ACCESS? ', ac ! ACCESS? SEQUENTIAL
        write (*,*) 'NUMBER? ', nu ! NUMBER? 25
    else
        write (*,*) "inquire-Fehler!"
    end if

    close(u)
end program test_io
