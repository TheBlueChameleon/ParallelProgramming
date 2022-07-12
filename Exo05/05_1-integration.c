#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float integrate_wrapper(float(func)(float),
                        float a, float b,
                        int intsteps);

float foo (float x) {
    float y = sin(x);
    return y*y;
}

int main () {
    float       result;
    const float exact = 0.2726756433f;

    result = integrate_wrapper(foo, 0, 1, 1000);
    printf("Fortran result  : %f\n", result);
    printf("exact result    : %f\n", exact);
    printf("rel. difference : %f%%\n", abs(result - exact) / exact * 100.);
}
