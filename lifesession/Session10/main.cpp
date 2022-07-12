#include <omp.h>
#include <iostream>
#include <sstream>
#include <string>

int global_int = -1;
#pragma omp threadprivate(global_int)

int main()
{
    omp_set_nested(true);

    #pragma omp parallel num_threads(3)
    {
        int n = -1;

        #pragma omp single copyprivate(n)
        {
            n = omp_get_thread_num();
            std::stringstream buffer;
            buffer << "Hello World (" << n << ")" << std::endl;
            std::cout << buffer.str();
        }

        int num = omp_get_thread_num();

        std::stringstream buffer;
        buffer << "Thread " << num << ", after single thread " << n << std::endl;

        std::cout << buffer.str();

        #pragma omp parallel default(none) shared(num, std::cout) num_threads(num + 1)
        {
            int num_inner = omp_get_thread_num();

            std::stringstream innerBuffer;
            innerBuffer << "  Outer Thread ID: " << num << ", inner Thread ID: " << num_inner << std::endl;

            std::cout << innerBuffer.str();
        }
    }

    std::cout << std::string(80, '=') << std::endl;

    // ====================================================================== //

    #pragma omp parallel num_threads(3) default(none) copyin(global_int) shared(std::cout)
    {
        int num = omp_get_thread_num();
        global_int = num;

        std::stringstream buffer;
        buffer << "Thread " << num << ": Hello World! global_int = " << global_int << std::endl;
        std::cout << buffer.str();
    }

    #pragma omp parallel num_threads(5)
    {
        std::stringstream buffer;
        buffer << "Thread " << omp_get_thread_num() << ": global_int = " << global_int << std::endl;
        std::cout << buffer.str();
    }
}
