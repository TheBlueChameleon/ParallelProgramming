#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <omp.h>

// ========================================================================== //

// Random Generator Utility Aggregate. Can be instantiated per thread and called to return a double in range [0., 1.)
class DefaultRandGen
{
    private:
        std::random_device               rd;
        std::mt19937                     gen;
        std::uniform_real_distribution<> dis;
        int uses = 0;

    public:
        DefaultRandGen() :
            rd(), gen(rd()), dis(0., 1.)
        {}

        // *INDENT-OFF*
        double operator()() {++uses; return dis(gen);}
        int getUses() const {return uses;}
        // *INDENT-ON*
};

// -------------------------------------------------------------------------- //
// fills vector input with random numbers in a parallelized fashion; asserts that for each element of the vector, the RNG has been called once.
void populate_parallelly(std::vector<double>& input)
{
    const size_t N = input.size();
    const size_t NThreads = omp_get_max_threads();
    std::vector<DefaultRandGen> randNumGenerators(NThreads);

    // parallelized populate
    #pragma omp parallel for default(none) shared(N, input, randNumGenerators)
    for (auto i = 0; i < N; ++i)
    {
        const auto threadID = omp_get_thread_num();
        input[i] = randNumGenerators[threadID]();
    }

    // ---------------------------------------------------------------------- //
    // assert correct functionality -- remove this in productive code
    auto accumulateUses = [] (const int acc, const DefaultRandGen& rng)
    {
        return acc + rng.getUses();
    };

    const int callsToRNG = std::accumulate(randNumGenerators.begin(), randNumGenerators.end(),
                                           0, accumulateUses);

    if (callsToRNG != N)
    {
        throw std::runtime_error("failed to populate the entire vector.");
    }
}

// -------------------------------------------------------------------------- //
void exo1_sections()
{
    const size_t N = 10000;
    std::vector<double> input1(N), input2(N), sum(N), product(N);

    populate_parallelly(input1);
    populate_parallelly(input2);

    #pragma omp parallel sections default(none), shared(N, input1, input2, sum, product) num_threads(3)
    {
        // .................................................................. //
        #pragma omp section
        std::transform(input1.begin(), input1.end(),
                       input2.begin(),
                       sum.begin(),
                       std::plus<>());

        // alternatively: classic index based loop
//        for (auto i=0; i<N; ++i)
//        {
//            sum[i] = input1[i] + input2[i];
//        }

        // .................................................................. //
        #pragma omp section
        std::transform(input1.begin(), input1.end(),
                       input2.begin(),
                       product.begin(),
                       std::multiplies<>());

        // alternatively: classic index based loop
//        for (auto i=0; i<N; ++i)
//        {
//            product[i] = input1[i] * input2[i];
//        }

        // .................................................................. //
        /* Implicit third section due to num_threads(3)
         * remains idle all the time.
         */
    }

    // ---------------------------------------------------------------------- //
    // assert correct behaviour:
    for (auto i=0u; i<N; ++i)
    {
        if (!(input1[i] + input2[i] == sum[i]))
        {
            std::cout << "At index " << i << ":" << std::endl;
            std::cout << input1[i] << " + " << input2[i] << " = " << input1[i] + input2[i] << "; got " << sum[i] << std::endl;
            throw std::runtime_error("failed to sum correctly");
        }

        if (!(input1[i] * input2[i] == product[i]))
        {
            std::cout << "At index " << i << ":" << std::endl;
            std::cout << input1[i] << " * " << input2[i] << " = " << input1[i] * input2[i] << "; got " << product[i] << std::endl;
            throw std::runtime_error("failed to sum correctly");
        }
    }

    std::cout << "part 1: sectons completed successfully." << std::endl;
}

// ========================================================================== //

using timed_result_t = std::tuple<
                       std::string,         // label
                       long long, long,     // sequential code: result, time
                       long long, long      // parallel code: result, time
                       >;
using fib_function   = std::function<long long (int)>;

// -------------------------------------------------------------------------- //
timed_result_t evaluateTimed(const std::string& label,
                             const fib_function& seq, const fib_function& par,
                             const int& ID)
{
    std::chrono::high_resolution_clock clock;

    decltype(clock.now()) tic, toc;
    long long rSeq, rPar;

    tic  = clock.now();
    rSeq = seq(ID);
    toc  = clock.now();
    const double tSeq = std::chrono::duration_cast<std::chrono::microseconds>(toc - tic).count();

    tic  = clock.now();
    rPar = par(ID);
    toc  = clock.now();
    const double tPar = std::chrono::duration_cast<std::chrono::microseconds>(toc - tic).count();

    return std::make_tuple(label, rSeq, tSeq, rPar, tPar);
}

void showResults(const std::vector<timed_result_t>& results)
{
    const auto heads = {"Run", "Sequential Result", "Time (us)", "Parallel Result", "Time (us)", "correct", "Speedup"};
    std::vector<size_t> widths(heads.size());

    // determine column widths
    std::transform(heads.begin(), heads.end(),
                   widths.begin(),
                   [] (const std::string& s)
    {
        return s.length();
    });

    std::stringstream stringbuffer;
    for (auto& result : results)
    {
        stringbuffer.str("");
        stringbuffer << std::fixed << std::setprecision(0) << std::get<1>(result);

        widths[0] = std::max(widths[0], std::get<0>(result).length());
    }

    // header output
    const auto fmtFlags = std::cout.flags();

    for (size_t i = 0; auto& head : heads)
    {
        std::cout << std::setw(widths[i]) << head << " | ";
        ++i;
    }
    std::cout << std::endl;

    for (size_t i = 0; i < heads.size(); ++i)
    {
        std::cout << std::string(widths[i] + (i != 0), '-') << "-+";
        // ... + (i != 0) : add extra dash in all but first column; this is equivalent to having "-+-" in all but the last column.
    }
    std::cout << std::endl;

    // result output
    std::cout << std::fixed << std::setprecision(0);
    for (auto& result : results)
    {
        const double speedup =static_cast<double>(std::get<2>(result)) / std::get<4>(result);
        std::cout << std::setw(widths[0]    ) << std::get<0>(result) << " | "
                  << std::setw(widths[1]    ) << std::get<1>(result) << " | "
                  << std::setw(widths[2]    ) << std::get<2>(result) << " | "
                  << std::setw(widths[3]    ) << std::get<3>(result) << " | "
                  << std::setw(widths[4]    ) << std::get<4>(result) << " | "
                  << std::setw(widths[5]    ) << (std::get<1>(result) == std::get<3>(result) ? "yes" : "no")           << " | "
                  << std::setw(widths[6] - 1) << std::setprecision(2) << speedup << "x |"
                  << std::setprecision(0) << std::endl;
    }

    std::cout.flags(fmtFlags);
}

// -------------------------------------------------------------------------- //
long long fib_sequential(int ID)
{
    // *INDENT-OFF*
    if      (ID == 0) {return 0;}
    else if (ID == 1) {return 1;}
    else              {return fib_sequential(ID - 1) + fib_sequential(ID - 2);}
    // *INDENT-ON*
}

// -------------------------------------------------------------------------- //
int biggestTID = -1, tsum = 0;
long long fib_parallel(int ID)
{

    if      (ID == 0)
    {
        return 0;
    }
    else if (ID == 1)
    {
        return 1;
    }
    else
    {
        long long first, second;

        #pragma omp parallel default(none) shared(first, second, ID, biggestTID, tsum)
        {
            #pragma omp nowait
            #pragma omp single
            {
                const auto tid = omp_get_thread_num();
                biggestTID = std::max(biggestTID, tid);
                tsum += tid;

                #pragma omp task
                {first = fib_parallel(ID - 1);}

                #pragma omp task
                {second = fib_parallel(ID - 2);}

                #pragma omp taskwait
            }
        }
        return first + second;
    }

}

// -------------------------------------------------------------------------- //
void exo2_tasks()
{
    const std::string labelStart = "run with i = ";

    std::vector<timed_result_t> results;

    for (auto i=15; i<25; ++i)
    {
        const std::string label = labelStart + std::to_string(i);
        std::cout << label << std::endl;

        results.push_back( evaluateTimed(label, fib_sequential, fib_parallel, i) );
    }

    showResults(results);
    std::cout << biggestTID << ", " << tsum << std::endl;

    for (const auto& result : results)
    {
        if (!(std::get<1>(result) == std::get<3>(result)))
        {
            throw std::runtime_error("failed to compute parallel result correctly.");
        }
    }

    std::cout << "part 2: tasks completed successfully." << std::endl;
}

// ========================================================================== //

void exo3_segfault()
{
    int n = 512;                        // originally: n = 1042, probably typo, meant to be 1024?
    int i, j, me, nthreads;

    // double A[n][n];                  // 1024 * 1024 * 8 Bytes = 8 MB exceeds stack space. Fails even in sequential mode

    /* The actual problem lies in the fact that without the parallel region, A is a pointer to a well allocated region on the stack.
     * private(A) creates an *uninitialized* threadlocal variable A, pointint to a random location. Subsequent write access based on A
     * is bound to give a segfault.
     *
     * In theory, this should be at least mitigated by using firstprivate(A), but printing the ptr A within and without the parallel
     * region did show different values. Why is that?
     */

    double A[3][n][n];           // ugly way of dealing with this: allocate enough memory for each thread before the parallel region

    #pragma omp parallel shared(nthreads, std::cout, n, A) private(i, j, me) num_threads(3) //firstprivate(A) -- why does this not work here?
    {
        // double A[n][n]; // having a threadlocal version *might* work, if n is small enough. Otherwise, there's the risk of overlap of the reserved adresses.
        // dynamically allocating the space proved to be prone to failure, too. The allocator is not thread-safe, it seems.

        // -- obtain and print info
        me       = omp_get_thread_num();
        nthreads = omp_get_num_threads();

        #pragma omp master
        {
            std::stringstream buffer;
            buffer << "Number of threads = " << nthreads << std::endl;
            std::cout << buffer.str();
        }

        std::stringstream buffer;
        buffer << "Thread " << me <<" starting..." << std::endl;
        std::cout << buffer.str();



        for (i = 0; i < n; ++i)
        {
            for (j = 0; j < n; ++j)
            {
                //A[i][j] = me + i + j;
                A[me][i][j] = me + i + j;
            }
        }

        // -- Print first and last element
        buffer.str("");
        buffer << "Thread " << me <<" done. First and last element = " << A[me][0][0] << ", " << A[me][n-1][n-1] << std::endl;
        std::cout << buffer.str();
    }

    std::cout << "part 3: segfault completed successfully" << std::endl;
}

// ========================================================================== //

void exo4_primes()
{
    const int max = 1000;
    std::vector<int> primes;
    std::vector<int> check = {3,   5,   7,  11,  13,  17,  19,  23,  29,  31,
                              37,  41,  43,  47,  53,  59,  61,  67,  71,  73,
                              79,  83,  89,  97, 101, 103, 107, 109, 113, 127,
                              131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
                              181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
                              239, 241, 251, 257, 263, 269, 271, 277, 281, 283,
                              293, 307, 311, 313, 317, 331, 337, 347, 349, 353,
                              359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
                              421, 431, 433, 439, 443, 449, 457, 461, 463, 467,
                              479, 487, 491, 499, 503, 509, 521, 523, 541, 547,
                              557, 563, 569, 571, 577, 587, 593, 599, 601, 607,
                              613, 617, 619, 631, 641, 643, 647, 653, 659, 661,
                              673, 677, 683, 691, 701, 709, 719, 727, 733, 739,
                              743, 751, 757, 761, 769, 773, 787, 797, 809, 811,
                              821, 823, 827, 829, 839, 853, 857, 859, 863, 877,
                              881, 883, 887, 907, 911, 919, 929, 937, 941, 947,
                              953, 967, 971, 977, 983, 991, 997
                             };

    bool skip = false;

    const int nThreads = omp_get_max_threads();
    const double delta = static_cast<double>(max) / nThreads;

    #pragma omp parallel default(none) shared(std::cout, max, primes, delta) private(skip)
    {
        const int iThread = omp_get_thread_num();
        const int start = iThread * delta;
        const int stop  = (iThread + 1) * delta;

        #pragma omp critical
        {
            std::cout << "Thread " << iThread << " works on range [" << start << ", " << stop << ")" << std::endl;
        }

        for (auto i = start; i < stop; ++i)
        {
            // *INDENT-OFF*
            skip = false;

            if (i == 1) {continue;}
            if (!(i&1)) {continue;} // skip even numbers

            for (auto divisor = 3; divisor < i; divisor += 2) {
                if (i % divisor == 0) {skip = true; break;}
            }

            if (skip) {continue;}
            else      {
                #pragma omp critical
                {primes.push_back(i);}
            }
            // *INDENT-ON*
        }
    }

    // ---------------------------------------------------------------------- //
    // assert correct behaviour:
    std::sort(primes.begin(), primes.end());

    if (primes != check)
    {
        std::vector<int> diff;
        std::set_difference(primes.begin(), primes.end(),
                            check.begin(), check.end(),
                            std::back_inserter(diff));
        std::cout << "found numbers differ on these values:" << std::endl;
        for (const auto& x : diff)
        {
            std::cout << x << std::endl;
        }
        throw std::runtime_error("failed to reproduce the primes.");
    }

    std::cout << "part 4: primes completed successfully" << std::endl;
}

// ========================================================================== //

int main()
{
    exo1_sections();
    exo2_tasks();
    exo3_segfault();
    exo4_primes();
}
