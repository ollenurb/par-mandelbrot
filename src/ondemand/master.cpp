#include "master.h"
#include <iostream>
#include "../utils/constants.h"
#include <mpi.h>

Master::Master(unsigned rank)
{
    this->rank = rank;
}

void Master::start()
{
    MPI_Status recv;
    while(true) {
        MPI_Recv(NULL, 0, MPI_CHAR, MPI_ANY_SOURCE, REQUEST_TAG, MPI_COMM_WORLD, &recv);
        std::cout << "Master: Got request from processor of rank " << recv.MPI_SOURCE << std::endl;
    }
}
