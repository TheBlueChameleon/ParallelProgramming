#include <cassert>
#include <iostream>
#include <numbers>

#include <omp.h>

#include "defaultrandgen.hpp"
#include "swarm.hpp"

Swarm::Swarm(int N, double timestamp)
{
    assert(N > 0);

    const auto NThreads = omp_get_max_threads();

    Swarm::coord_t r, v;
    std::vector<DefaultRandGen> rngs(NThreads);

    // note: parallelizing this means trouble for computing the energy..?
    //#pragma omp parallel for
    for (int i = 0; i < N; ++i)
    {
        const auto tID = omp_get_thread_num();

        r.x = rngs[tID]() * xMax;
        r.y = rngs[tID]() * yMax;

        v.x = (rngs[tID]() - .5) * xMax / 10.;
        v.y = (rngs[tID]() - .5) * yMax / 10.;

        #pragma omp critical
        {
            addParticle(r, v);
        }
    }

    this->timestamp = timestamp;
}

double Swarm::getTimestamp() const
{
    return timestamp;
}

const std::vector<Swarm::coord_t>& Swarm::getVelocities() const
{
    return velocities;
}

const std::vector<Swarm::coord_t>& Swarm::getCoordinates() const
{
    return coordinates;
}

const std::vector<Swarm::coord_t>& Swarm::getAccelerations() const
{
    return accelerations;
}

double Swarm::getEKin() const
{
    return .5 * std::accumulate(velocities.begin(), velocities.end(),
                                0.,
                                [] (const auto acc, const auto elm)
    {
        return acc + (elm.x * elm.x  +  elm.y * elm.y);
    });
}

double Swarm::getEPot() const
{
    double reVal = 0.;

    for (    int i =     0; i < coordinates.size(); ++i)
    {
        for (int j = i + 1; j < coordinates.size(); ++j)
        {
            reVal += interactionBetween(i, j);
        }
    }

    return reVal;
}

double Swarm::getETot() const
{
    auto eKin = getEKin();
    auto ePot = getEPot();
    return eKin + ePot;
}

void Swarm::addParticle(const Swarm::coord_t& coordinate, const coord_t& velocity)
{
    coordinates      .push_back(coordinate);
    velocities       .push_back( velocity );
    accelerations    .push_back( {0., 0.} );

    nextCoordinates  .push_back(coordinate);
    nextVelocities   .push_back( velocity );
    nextAccelerations.push_back( {0., 0.} );
}

double Swarm::distanceBetween(int i, int j) const
{
    assert(i >= 0);
    assert(j >= 0);
    assert(i < coordinates.size());
    assert(j < coordinates.size());

    const auto& ri = coordinates[i];
    const auto& rj = coordinates[j];

    const auto dx = ri.x - rj.x;
    const auto dy = ri.y - rj.y;

    return std::sqrt(dx*dx + dy*dy);
}

double Swarm::interactionBetween(int i, int j) const
{
    const auto r = std::min(distanceBetween(i, j), cutoff);
    const auto sin = std::sin(r);
    return sin*sin;
}

Swarm::coord_t Swarm::forceBetween(int i, int j) const
{
    Swarm::coord_t reVal = {0, 0};
    double rAbs = distanceBetween(i, j);

    if (rAbs < cutoff)
    {
        const auto& ri = coordinates[i];
        const auto& rj = coordinates[j];

        const auto trig = std::sin(rAbs) * std::cos(rAbs);

        reVal = {ri.x - rj.x, ri.y - rj.y};
        reVal.x *= -2. / rAbs * trig;
        reVal.y *= -2. / rAbs * trig;
    }

    return reVal;
}

Swarm::coord_t Swarm::forceOn(int i) const
{
    Swarm::coord_t reVal = {0, 0};

    for (auto j = 0; j < coordinates.size(); ++j)
    {
        if (i == j)
        {
            continue;
        }

        Swarm::coord_t summand;
        summand = forceBetween(i, j);

        reVal.x += summand.x;
        reVal.y += summand.y;
    }

    return reVal;
}

//#define NOFORCE
void Swarm::evolve()
{
    #pragma omp parallel for
    for (auto i = 0; i < coordinates.size(); ++i)
    {
        auto&  ri = coordinates      [i];
        auto& nri = nextCoordinates  [i];
        auto&  vi = velocities       [i];
        auto& nvi = nextVelocities   [i];
        auto&  ai = accelerations    [i];
        auto& nai = nextAccelerations[i];

#ifndef NOFORCE
        const auto dt_squared_halves = .5 * dt * dt;

        // do coordinate update wrt. acceleration
        nri.x = ri.x + ai.x * dt_squared_halves;
        nri.y = ri.y + ai.y * dt_squared_halves;

        // compute next force/acceleartion
        auto F = forceOn(i);
        nai.x = F.x * dt_squared_halves;
        nai.y = F.y * dt_squared_halves;

        // do velocities update
        nvi.x = vi.x + .5 * (ai.x + nai.x);
        nvi.y = vi.y + .5 * (ai.y + nai.y);
#endif

        // do coordinates update wrt velocity
        nri.x = ri.x + vi.x * dt;
        nri.y = ri.y + vi.y * dt;

        // reflect at vertical boundaries
        if (nri.x < 0)
        {
            nri.x = -nri.x;
            nvi.x = -nvi.x;
            //vi.x  = - vi.x;
        }
        if (nri.x > xMax)
        {
            nri.x = 2 * xMax - nri.x;
            nvi.x = -nvi.x;
        }

        // reflect at horizontal boundaries
        if (nri.y < 0)
        {
            nri.y = -nri.y;
            nvi.y = -nvi.y;
            //vi.y  = - vi.y;
        }
        if (nri.y > yMax)
        {
            nri.y = 2 * yMax - nri.y;
            nvi.y = -nvi.y;
        }
    }

    // update active dataset
    coordinates  .swap(nextCoordinates);
    velocities   .swap(nextVelocities);
    accelerations.swap(nextAccelerations);

    // tic toc
    timestamp += dt;
}
