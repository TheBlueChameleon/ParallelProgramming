#include <cassert>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <vector>

#include <omp.h>

#include "interfaces.hpp"
#include "defaultrandgen.hpp"


double getPiMCParallel(const size_t N, size_t NThreads = 0)
{
    if (NThreads < 1)
    {
        NThreads = omp_get_max_threads();
    }

    std::vector<DefaultRandGen> randGens(NThreads);

    size_t acc = 0;

    #pragma omp parallel for default(none) shared(randGens, N) reduction(+:acc) num_threads(NThreads)
    for (size_t i = 0; i < N; ++i)
    {
        const size_t threadID = omp_get_thread_num();
        double x, y;

        x = randGens[threadID]();
        y = randGens[threadID]();

        acc += (x*x + y*y) < 1.;
    }

    return 4. * static_cast<double>(acc) / N;
}



void task_01_montecarlo()
{
    show_header("MONTE CARLO ALGORITHM");

    const size_t N = 10'000'000;
    const size_t NMaxThreads = omp_get_max_threads();

    const double epsilon = 0.05;
    double piApprox, delta;
    bool   allAcceptable = true, thisAcceptable;

    std::cout << "NumThreads | Approximation | Deviation | Acceptable" << std::endl;
    std::cout << "-----------+---------------+-----------+-----------" << std::endl;
    std::cout << std::boolalpha << std::fixed;
    for (auto NThreads = 1u; NThreads < NMaxThreads; ++NThreads)
    {
        piApprox       = getPiMCParallel(N, NThreads);
        delta          = std::abs(piApprox - std::numbers::pi);
        thisAcceptable = delta < epsilon;

        std::cout << NThreads << "          | "
                  << piApprox   << "      | "
                  << delta      << "  | "
                  << thisAcceptable << std::endl;

        allAcceptable &= thisAcceptable;
    }

    assert(allAcceptable);
}
