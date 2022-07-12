#!/bin/bash

clear
echo "removing potentially existing file foo.bar ..."
rm foo.bar
echo "... done"

echo "attempting to compile..."
gfortran 05_2-mandelbrot.f08
echo "... done"

echo "compiled. attempting to execute ..."
./a.out
echo "... done. Press enter to terminate"

gnuplot plotscript.gnuplot
