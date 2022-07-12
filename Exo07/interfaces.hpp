#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include <chrono>
#include <string>

void show_header(const std::string& headline);

extern std::chrono::high_resolution_clock timeDevice;
using timepoint_t = decltype(timeDevice.now());
using timespan_t  = std::chrono::duration<float, std::chrono::milliseconds::period>;

timepoint_t getNow();
double getTimeBetweenInMilliSeconds(timepoint_t tic, timepoint_t toc);
double getTimeSinceInMilliSeconds(timepoint_t tic);

void task_01_montecarlo();
void task_02_matrices();
void task_03_mandelbrot();

#endif // INTERFACES_HPP
