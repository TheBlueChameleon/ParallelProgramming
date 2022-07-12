#include <cassert>
#include <fstream>
#include <tuple>
#include <vector>

#include <omp.h>

#include "interfaces.hpp"
#include "defaultmatrix.hpp"
#include "sparsematrix.hpp"

void unittest_DefaultMatrix()
{
    const int N = 6;
    DefaultMatrix M(N, N);                      // inits to null matrix
    std::vector<double> data(N, 1), result;     // inits to [1, 1, ..., 1]

    result = M * data;                          // should give null vector
    assert( std::all_of(result.begin(), result.end(),
                        [&N] (const auto& x)
    {
        return x == 0;
    }) );


    for ( auto& x : M.exposeValues() )          // set all elements to 1.
    {
        x = 1.;
    }

    result = M * data;                          // should give [N, N, ..., N]
    assert( std::all_of(result.begin(), result.end(),
                        [&N] (const auto& x)
    {
        return x == N;
    }) );
}

void unittest_SparseMatrix()
{
    const int N = 6;
    DefaultMatrix M(N, N,
    {
        10, 0, 0, 0, -2, 0,
        3, 9, 0, 0, 0, 3,
        0, 7, 8, 7, 0, 0,
        3, 0, 8, 7, 5, 0,
        0, 8, 0, 9, 9, 13,
        0, 4, 0, 0, 2, -1,
    });
    SparseMatrix S(M);

    std::vector<double> expectedValues = {10., -2., 3., 9., 3., 7., 8., 7., 3., 8., 7., 5., 8., 9., 9., 13., 4., 2., -1.};
    std::vector<size_t> expectedValCol = {0, 4, 0, 1, 5, 1, 2, 3, 0, 2, 3, 4, 1, 3, 4, 5, 1, 4, 5},
                        expectedValRow = {0, 2, 5, 8, 12, 16, 19};

    assert( S.getValues() == expectedValues );
    assert( S.getValCol() == expectedValCol );
    assert( S.getValRow() == expectedValRow );

    std::vector<double> vector(N, 1), result, expectedResult = M * vector;

    result = S * vector;
    assert( result == expectedResult );

    result = M * vector;
    assert( result == expectedResult );
}

using timingResult_t = std::tuple<
                       size_t,              // matrix size
                       size_t,              // number of threads
                       double,              // wall time: DefaultMatrix
                       double               // wall time: SparseMatrix
                       >;

timingResult_t matmulTest(size_t N, size_t NThreads)
{
    const auto oldMax = omp_get_max_threads();
    omp_set_max_active_levels(NThreads);

    DefaultMatrix M(N, N);
    M.populateRandomly(.1);
    SparseMatrix S(M);

    const auto vector = std::vector<double>(N, 1.);
    std::vector<double> resultD, resultS;

    const auto ticD = getNow();
    resultD = M * vector;
    const auto tocD = getNow();

    const auto ticS = getNow();
    resultS = S * vector;
    const auto tocS = getNow();

    assert(resultD == resultS);

    omp_set_max_active_levels(oldMax);

    return timingResult_t(N, NThreads,
                          getTimeBetweenInMilliSeconds(ticD, tocD),
                          getTimeBetweenInMilliSeconds(ticS, tocS)
                         );
}

void task_02_matrices()
{
    show_header("MATRIX MULTIPLICATION");

    unittest_DefaultMatrix();
    unittest_SparseMatrix();
    std::cout << "unit tests passed. ready to take action." << std::endl;

    std::vector<timingResult_t> results;

    for (const auto N :
            {
                2, 5, 10, 50, 100, 500, 1000
            })
    {
        for (const auto NThreads :
                {
                    1, 2, 4, 8
                })
        {
            std::cout << "running benchmark for N = " << N << ", NThreads = " << NThreads << " ... " << std::flush;
            results.push_back( matmulTest(N, NThreads) );
            std::cout << "done." << std::endl;
        }
    }

    std::string filename = "matrices.bar";
    std::cout << "all benchmarks done. Writing to file '" << filename << "'." << std::endl;

    std::ofstream hFile(filename);
    assert(hFile.is_open());

    for (const auto& result : results)
    {
        hFile << std::get<0>(result) << "  "
              << std::get<1>(result) << "  "
              << std::get<2>(result) << "  "
              << std::get<3>(result) << std::endl;
    }
}
