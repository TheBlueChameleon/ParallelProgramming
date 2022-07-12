#include <chrono>
#include <iomanip>
#include <iostream>

#include "interfaces.hpp"

void show_header(const std::string& headline)
{
    const size_t len = headline.size();
    const size_t spaceL = (78 - len) >> 1;
    const size_t spaceR = 78 - (len + spaceL);

    std::cout << "+" << std::string(78, '-') << "+" << std::endl;
    std::cout << "|" << std::string(spaceL, ' ') << headline << std::string(spaceR, ' ') << "|" << std::endl;
    std::cout << "+" << std::string(78, '-') << "+" << std::endl;
}

std::chrono::high_resolution_clock timeDevice;

timepoint_t getNow()
{
    return timeDevice.now();
}

double getTimeBetweenInMilliSeconds(timepoint_t tic, timepoint_t toc)
{
    const timespan_t delta = toc - tic;
    return delta.count();
}

double getTimeSinceInMilliSeconds(timepoint_t tic)
{
    const timespan_t delta = timeDevice.now() - tic;
    return delta.count();
}
