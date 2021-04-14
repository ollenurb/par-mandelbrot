#ifndef MASTER_H
#define MASTER_H

#include <vector>

class Master {
    public:
        Master(unsigned, unsigned);
        void start();
        const std::vector<int>& get_results();

    private:
        unsigned img_dim;
        unsigned block_sz;
        int n_workers;
        std::vector<int> displ;
        std::vector<int> results;
};

#endif
