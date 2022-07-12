#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

#include "trajectory.hpp"
#include "swarm.hpp"

// ========================================================================== //

void unittest_Trajectory()
{
    Trajectory trajectory;

    for (double alpha = 0.; alpha < 6.28; alpha += .1)
    {
        Swarm::coord_t coordinate, velocity;
        Swarm          swarm;

        coordinate.x = std::cos(alpha);
        coordinate.y = std::sin(alpha);
        swarm.addParticle(coordinate, velocity);

        coordinate.x = std::cos(alpha + 3.14);
        coordinate.y = std::sin(alpha + 3.14);
        swarm.addParticle(coordinate, velocity);

        trajectory.addFrame(swarm);
    }

    trajectory.writeCoordinates("dummy.out");
}

void unittest_SwarmEvolve()
{
    constexpr double tMax = 10.;

    Trajectory  trajectory;
    Swarm       swarm;
    double      timestamp = swarm.getTimestamp();

    swarm.addParticle({5.0, 9.5}, {+0.3, +0.9});
    swarm.addParticle({5.0, 0.5}, {+0.3, -0.9});
    swarm.addParticle({9.5, 5.0}, {+0.9, +0.3});
    swarm.addParticle({0.5, 5.0}, {-0.9, +0.3});

    trajectory.addFrame(swarm);

    while (timestamp < tMax)
    {
        swarm.evolve();
        trajectory.addFrame(swarm);
        timestamp = swarm.getTimestamp();
    }

    trajectory.writeEnergies   ("energies.log");
    trajectory.writeCoordinates("coordinates.log");

    std::system("gnuplot plot.gnuplot");
}

// ========================================================================== //

void simulation()
{
    constexpr double tMax = 100.;
    std::cout << "ABOUT TO RUN SIMULATION..." << std::endl;

    Swarm       swarm(500);
    Trajectory  trajectory;
    double      timestamp;

    trajectory.addFrame(swarm);

    std::cout << std::fixed << std::setprecision(1);
    do
    {
        swarm.evolve();
        trajectory.addFrame(swarm);

        timestamp = swarm.getTimestamp();
        std::cout << "  t = " << timestamp << std::endl;
    }
    while (timestamp < tMax);

    std::cout << "SIMULATION FINISHED. ABOUT TO WRITE LOGS..." << std::endl;

    trajectory.writeEnergies   ("energies.log");
    trajectory.writeCoordinates("coordinates.log");

    const auto& energies = trajectory.getEnergies();

    const double eStd = energies[0];
    const double eMax = *std::max_element(energies.begin(), energies.end());
    const double eMin = *std::min_element(energies.begin(), energies.end());

    const double deltaUp = std::abs(eMax - eStd) / eStd;
    const double deltaDn = std::abs(eMin - eStd) / eStd;

    std::cout << "  Peak   energy at " << (deltaUp * 100.) << "% from start energy; " << (deltaUp <  0.03 ? "" : "NOT ") << "within bounds" << std::endl;
    std::cout << "  Valley energy at " << (deltaDn * 100.) << "% from start energy; " << (deltaUp > -0.03 ? "" : "NOT ") << "within bounds" << std::endl;

    std::cout << "WRITING LOGS FINISHED. ABOUT TO PLOT..." << std::endl;

    std::system("gnuplot plot.gnuplot");

    std::cout << "ALL DONE." << std::endl;
}

// ========================================================================== //

int main (int argc, char** argv)
{
//    unittest_Trajectory();
//    unittest_SwarmEvolve();
    simulation();
}
