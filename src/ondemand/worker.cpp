#include "worker.h"
#include "../utils/constants.h"
#include "../utils/mandelbrot.h"
#include <mpi.h>
#include <iostream>
#include <vector>

void Worker::compute_mandelbrot(int size) {
    int x, y;
    double x_0, y_0;

    for(int i = 0; i < size; i++) {
        x = (from_to[0] + i) % width;
        y = (from_to[1] + i) / width;
        x_0 = scale_interval(0, width, -2.5, 1, x);
        y_0 = scale_interval(0, height, -1, 1, y);
        result[i] = compute_point(x_0, y_0);
    }
}

Worker::Worker(unsigned master_rank, unsigned const width, unsigned const height)
{
    this->master_rank = master_rank;
    this->width = width;
    this->height = height;
    MPI_Comm_rank(MPI_COMM_WORLD, &this->rank);
}

void Worker::start()
{
    int res_sz;
    while(request_data()) {
        res_sz = from_to[1] - from_to[0];
        if(result.size() < res_sz) {
            result.resize(res_sz);
        }

        /* Do the actual computation */
        compute_mandelbrot(res_sz);

        /* Send result back to worker */
        MPI_Send(result.data(), res_sz, MPI_INT, master_rank, DATA_TAG, MPI_COMM_WORLD);
    }
    std::cout << "Worker " << rank << " finished, terminating.." << std::endl;
}

/*
 * Request data from the master process.
 * When the tag of the response from the master process is equals to DATA_TAG
 * return true and fill the from_to buffer with the received data, otherwise
 * returns false.
 */
bool Worker::request_data()
{
    int msg_tag;
    MPI_Status msg_status;

    MPI_Send(NULL, 0, MPI_CHAR, master_rank, REQUEST_TAG, MPI_COMM_WORLD);
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &msg_status);
    msg_tag = msg_status.MPI_TAG;

    if(msg_tag != ASSIGN_TAG && msg_tag != STOP_TAG) {
        std::cerr << "Worker: Received an unexpected message tag from master: "
                  << msg_status.MPI_TAG
                  << ", terminating.."
                  << std::endl;

        exit(EXIT_FAILURE);
    }
    if(msg_tag == ASSIGN_TAG) {
        MPI_Recv(&from_to, 2, MPI_UNSIGNED, master_rank, ASSIGN_TAG, MPI_COMM_WORLD, &msg_status);
        std::cout << "Worker: "
                  << rank
                  << ", received an assignment from master: "
                  << from_to[0] << " - " << from_to[1]
                  << ", diff: " << from_to[1] - from_to[0]
                  << std::endl;
        return true;
    }
    else {
        MPI_Recv(NULL, 0, MPI_UNSIGNED, master_rank, STOP_TAG, MPI_COMM_WORLD, NULL);
        return false;
    }
}


