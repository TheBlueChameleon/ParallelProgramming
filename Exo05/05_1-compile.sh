#!/bin/bash

clear
gfortran -c 05_1-integration.f08
gcc 05_1-integration.c 05_1-integration.o -lm -lgfortran && ./a.out
