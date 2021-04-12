#ifndef WORKER_H
#define WORKER_H

#include <vector>

class Worker {
    public:
        Worker(unsigned, const unsigned, const unsigned);
        void start();

    private:
        bool request_data();
        /* will contain both lower and upper bound of the image's partition to compute */
        unsigned from_to[2];
        std::vector<double> result;
        unsigned width, height;
        unsigned master_rank;
};

#endif
