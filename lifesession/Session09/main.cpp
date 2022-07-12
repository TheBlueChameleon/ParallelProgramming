#include <iostream>
#include <sstream>

#include <omp.h>

int main()
{
    int nthread = -1;
    int ishared = -1;

    omp_set_num_threads(2);

    #pragma omp parallel num_threads(10) default(none) private(nthread) shared(std::cout, ishared)
    {
        nthread = omp_get_thread_num();

        std::ostringstream buffer;
        buffer << "Hello World from thread #" << nthread << " sharing ishared = " << ishared << std::endl;

        std::cout << buffer.str();
    }

    std::cout << "nthread: " << nthread << std::endl;
    std::cout << "ishared: " << ishared << std::endl;
    return 0;
}
