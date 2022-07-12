#include <algorithm>
#include <sstream>
#include <vector>

#include <mpi.h>

#include "mpiHelpers.hpp"

void task02_groupComm() {
    const int debug = 1;

    const std::vector<int> group0_ranks = {1, 2};
    const std::vector<int> group1_ranks = {0, 3};

    std::stringstream s;

    const auto groupID = std::find(group1_ranks.begin(), group1_ranks.end(), procID) != group1_ranks.end();

    if (debug)  {
        s   << "(02) World process #" << procID
            << " is in group " << groupID << std::endl;
        showAndClear(s);
    }

    MPI_Group group;
    MPI_Comm_group(MPI::COMM_WORLD, &group);

    MPI_Group_incl(group, 2, (groupID ? group1_ranks.data() : group0_ranks.data()), &group);

    MPI_Comm commGrp;
    MPI_Comm_create(MPI::COMM_WORLD, group, &commGrp);

    int result = 0;
    MPI_Allreduce(&procID, &result,
                  1, MPI_INTEGER,
                  MPI_SUM,
                  commGrp);

    s   << "(02) World process #" << procID
            << " in group " << groupID
            << " received " << result
            << std::endl;
    showAndClear(s);
}
