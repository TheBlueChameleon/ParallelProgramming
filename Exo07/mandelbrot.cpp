#include <cassert>
#include <complex>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <omp.h>

#include "interfaces.hpp"

const size_t loopLimit = 50;
const double divergenceLimit = 2.;
const size_t Nx = 2000, Ny = 1000;

using complex_t = std::complex<double>;

bool isInMandelbrotSet(complex_t z)
{
    complex_t c = z;

    for (size_t i = 0u; i < loopLimit; ++i)
    {
        z = z*z + c;

        if ( std::abs(z) > divergenceLimit )
        {
            return false;
        }
    }

    return true;
}

bool isInMandelbrotSet(const double x, const double y)
{
    return isInMandelbrotSet( complex_t(x, y) );
}

double getCoordFromIdx(size_t i, size_t N, double offset)
{
    return (static_cast<double>(i) / N) + offset;
}

double getXCoordFromIdx(size_t i)
{
    return getCoordFromIdx(i, Nx, -1.5);
}

double getYCoordFromIdx(size_t i)
{
    return getCoordFromIdx(i, Ny, -0.5);
}

void writeListTo (std::ofstream& hFile)
{
    using coordinate_t = std::pair<double, double>;
    std::vector<coordinate_t> points;

    double x, y;
    #pragma omp parallel for default(none) private(x, y) shared(Nx, Ny, hFile, points) collapse(2)
    for (size_t i = 0; i < Nx; ++i)
    {
        for (size_t j = 0; j < Ny; ++j)
        {
            x = getXCoordFromIdx(i);
            y = getYCoordFromIdx(j);

            if (isInMandelbrotSet(x, y))
            {
                // not necessary if pre-allocated; compute ID from i and j
                #pragma omp critical
                {
                    points.push_back( coordinate_t(x, y) );
                }
            }
        }
    }

    std::sort(points.begin(), points.end(),
              [] (const coordinate_t& a, const coordinate_t& b)
              // *INDENT-OFF*
              {
                  if      (a.first  < b.first) {return true;}
                  else if (a.first == b.first)
                  {
                      if (a.second < b.second) {return true;}
                  }
                  return false;
    });
    // *INDENT-ON*

    for (const auto& pt : points)
    {
        hFile << pt.first << "  " << pt.second << std::endl;
    }

}

void task_03_mandelbrot()
{
    show_header("MANDELBROT SET");

    std::string filename = "mandelbrot.dat";

    std::ofstream hFile;
    hFile.open(filename);

    assert( hFile.is_open() );

    writeListTo(hFile);

    std::cout << "see file '" << filename << "' for results." << std::endl;
    std::cout << "attempting to invoke gnuplot ..." << std::endl;

    std::system("gnuplot ./mandelbrot.gnuplot");
}
