#include "master.h"
#include <iostream>
#include "../utils/constants.h"
#include <mpi.h>

Master::Master(unsigned width, unsigned height)
{
    this->img_dim = width*height;
    // Size of a single block (defines the number of elements to be computed from the worker)
    this->block_sz = img_dim / N_BLOCKS;
    MPI_Comm_size(MPI_COMM_WORLD, &this->n_workers);
    this->n_workers -= 1;
    this->displ.resize(this->n_workers);
    this->results.resize(this->img_dim);
}

void Master::start()
{
    MPI_Status msg_status;
    unsigned assigned = 0, received = 0;
    int msg_src, msg_tag;

    while(assigned < N_BLOCKS || received < N_BLOCKS) {
        std::cout << "Master: Waiting for incoming messages.."
                  << std::endl;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &msg_status);
        msg_src = msg_status.MPI_SOURCE;
        msg_tag = msg_status.MPI_TAG;

        /* Check unhandled message tags */
        if(msg_tag != REQUEST_TAG && msg_tag != DATA_TAG) {
            std::cerr << "Master: Received an unexpected message tag: "
                      << msg_status.MPI_TAG
                      << ", terminating.."
                      << std::endl;
            exit(EXIT_FAILURE);
        }

        /* If the message is a request of work, then assign work
         * to the worker, otherwise save the incoming data according
         * to the displacements vector.
         */
        if(msg_tag == REQUEST_TAG) {
            MPI_Recv(NULL, 0, MPI_CHAR, MPI_ANY_SOURCE, REQUEST_TAG, MPI_COMM_WORLD, NULL);
            unsigned from_to[2] = {assigned*img_dim/N_BLOCKS, (assigned+1)*img_dim/N_BLOCKS};

            /* Send back a work assignment */
            std::cout << "Master: Assigning task to " << msg_src << std::endl;

            MPI_Send(&from_to, 2, MPI_UNSIGNED, msg_src, ASSIGN_TAG, MPI_COMM_WORLD);
            displ[msg_src-1] = from_to[0];
            assigned += 1;
        }
        else { // Otherwise incoming data has to be collected with MPI_Recv
            int msg_sz;
            int *recvbuf = (results.data() + displ[msg_src-1]);
            MPI_Get_count(&msg_status, MPI_INT, &msg_sz);
            MPI_Recv(recvbuf, msg_sz, MPI_INT, MPI_ANY_SOURCE, DATA_TAG, MPI_COMM_WORLD, NULL);
            received += 1;
        }
    }

    std::cout << "Master: Sending a stop message to every worker..." << std::endl;
    for(int i = 1; i <= n_workers; i++) {
        MPI_Send(NULL, 0, MPI_UNSIGNED, i, STOP_TAG, MPI_COMM_WORLD);
    }

}


