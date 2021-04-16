#include "../utils/palette.h"
#include "../utils/bitmap.h"
#include "../utils/constants.h"
#include "master.h"
#include "worker.h"
#include <mpi.h>
#include <errno.h>
#include <iostream>

#define MASTER_RANK 0
#define PALETTE_PATH "palettes/volcano.map"

int main(int argc, char** argv)
{
    int size, rank;
    unsigned width = 2560, height = 1440;
    double t0, t1;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(size < 2) {
        std::cerr << "On-demand parallel computations requires the number of processes to be > 1"
                  << std::endl;
        exit(EINVAL);
    }

    /* Master and worker differentiation */
    if(rank == MASTER_RANK) {
        Master m(width, height);
        t0 = MPI_Wtime();
        m.start();
        t1 = MPI_Wtime();

        /* Create the actual image */
        std::string filename = "dyn_mandel.bmp";
        Bitmap mandel(width, height);
        Palette palette(MAX_ITER);
        rgb color;

        palette.load_from_file(PALETTE_PATH);

        /* Transform the image from number of iterations to actual pixel values */
        int n_pixels = width*height;
        std::vector<int> results = m.get_results();
        for(int i = 0; i < n_pixels; i++) {
            color = palette[results[i]];
            mandel.set_pixel(i, &color);
        }

        mandel.write_to_file(filename);
        std::cout << "Dynamic implementation took " << t1 - t0 << " seconds" << std::endl;
    }
    else {
        Worker w(MASTER_RANK, width, height);
        w.start();
    }

    MPI_Finalize();
}
