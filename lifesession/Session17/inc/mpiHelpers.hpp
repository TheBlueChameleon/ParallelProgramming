#include <chrono>
#include <random>
#include <sstream>

// ========================================================================== //
// MPI Interface Utility

extern int procID, Nprocs;

void MPI_Setup (int argc, char ** argv, bool verbose = true);

// ========================================================================== //
// Timing Utility

extern std::chrono::high_resolution_clock timeDevice;

using timepoint_t = decltype(timeDevice.now());
using timespan_t  = std::chrono::duration<float, std::chrono::milliseconds::period>;

timepoint_t getNow();
double getTimeBetweenInMilliSeconds(timepoint_t tic, timepoint_t toc);
double getTimeSinceInMilliSeconds(timepoint_t tic);

// ========================================================================== //
// Randm Number Utility

extern std::random_device               _random_device;
extern std::mt19937                     _random_generator;
extern std::uniform_real_distribution<> _random_distribution;

double getRandomBetween(const double a, const double b);
int getRandomIntBetween(const int a, const int b);

// ========================================================================== //
// Misc

void showAndClear(std::stringstream& buffer);
