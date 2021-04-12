#include "worker.h"
#include <mpi.h>
#include "../utils/constants.h"
#include <iostream>
#include <vector>

Worker::Worker(unsigned master_rank, unsigned const width, unsigned const height)
{
    this->master_rank = master_rank;
    this->width = width;
    this->height = height;
}

void Worker::start()
{
    while(request_data()) {
        /* Do the actual computation */
    }
}

/*
 * Request data from the master process.
 * When the tag of the response from the master process is equals to DATA_TAG
 * return true and fill the from_to buffer with the received data, otherwise
 * returns false.
 */
bool Worker::request_data()
{
    MPI_Status msg_status;
    MPI_Send(NULL, 0, MPI_CHAR, master_rank, REQUEST_TAG, MPI_COMM_WORLD);
    MPI_Recv(&from_to, 2, MPI_UNSIGNED, master_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &msg_status);
    int msg_tag = msg_status.MPI_TAG;

    if(msg_tag != ASSIGN_TAG && msg_tag != STOP_TAG) {
        std::cerr << "Worker: Received an unexpected message tag from master: "
                  << msg_status.MPI_TAG
                  << ", terminating.."
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        return msg_tag == ASSIGN_TAG;
    }
}
