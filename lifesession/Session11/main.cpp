#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <omp.h>

int main()
{
    const int n = 20, m = 20;

    #pragma omp parallel default(none) shared(n, std::cout)
    {
        #pragma omp for
        for (int i=0; i<n; ++i)
        {
            std::stringstream buffer;
            buffer.str("");
            buffer << "i = " << i << ", thread is #" << omp_get_thread_num() << std::endl;
            std::cout << buffer.str();
        }
    }
    std::cout << std::string(80, '-') << std::endl;

    #pragma omp parallel for default(none) shared(n, std::cout)
    for (int i=0; i<n; ++i)
    {
        std::stringstream buffer;
        buffer.str("");
        buffer << "i = " << i << ", thread is #" << omp_get_thread_num() << std::endl;
        std::cout << buffer.str();
    }
    std::cout << std::string(80, '=') << std::endl;


    #pragma omp parallel for default(none) shared(n, m, std::cout) collapse(2)
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            std::stringstream buffer;
            buffer.str("");
            buffer << "i = " << i << "; "
                   << "j = " << j << "; "
                   << "thread is #" << omp_get_thread_num() << std::endl;
            std::cout << buffer.str();
        }
    }
    std::cout << std::string(80, '=') << std::endl;

    int j;
    #pragma omp parallel for lastprivate(j)
    for (int i=0; i<n; ++i)
    {
        j = i;

        std::stringstream buffer;
        buffer.str("");
        buffer << "i = " << i << "; "
               << "thread is #" << omp_get_thread_num() << std::endl;
        std::cout << buffer.str();
    }
    std::cout << j << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    #pragma omp parallel default(none) shared(n, std::cout)
    {
        #pragma omp for ordered
        for (int i=0; i<n; ++i)
        {
            #pragma omp ordered
            {
                std::stringstream buffer;
                buffer.str("");
                buffer << "i = " << i << ", thread is #" << omp_get_thread_num() << std::endl;
                std::cout << buffer.str();
            }
        }
    }
    std::cout << std::string(80, '=') << std::endl;


    const int N = 20000;
    std::vector<int> v(N);
    double t1, t2;

    t1 = std::clock();
    #pragma omp parallel for
    for (int i=0; i<N; ++i)
    {
        v[i] = i;
    }
    t2 = std::clock();
    std::cout << t2 - t1 << " us" << std::endl;

    t1 = omp_get_wtime();
    #pragma omp parallel for
    for (int i=0; i<N; ++i)
    {
        v[i] = i;
    }
    t2 = omp_get_wtime();
    std::cout << int(1E6 * (t2 - t1)) << " us" << std::endl;


    return 0;
}
