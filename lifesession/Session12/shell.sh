#!/bin/bash

arg="foo"

if [ $1 == "1" ];
then
    echo "bye"
else
    echo $1
fi

gfortran -fopenmp -o prog prog.f90
export OMP_NUM_THREADS=10
./prog $arg > log.txt

echo "#####################"
echo  showing file 'log.txt':
echo "#####################"
cat log.txt
