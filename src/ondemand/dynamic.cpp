#include "../utils/palette.h"
#include "../utils/bitmap.h"
#include "master.h"
#include "worker.h"
#include <mpi.h>
#include <errno.h>
#include <iostream>

#define MASTER_RANK 0

int main(int argc, char** argv)
{
    int size, rank;
    unsigned width = 200, height = 200;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(size < 2) {
        std::cerr << "On-demand parallel computations requires the number of processes to be > 1"
                  << std::endl;
        return EINVAL;
    }

    /* Master and worker differentiation */
    if(rank == MASTER_RANK) {
        Master m(width, height);
        m.start();
        /* Create the actual image */
    }
    else {
        Worker w(MASTER_RANK, width, height);
        w.start();
    }

    MPI_Finalize();
}
