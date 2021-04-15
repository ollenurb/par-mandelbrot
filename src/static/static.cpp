#include <iostream>
#include <string>
#include <mpi.h>
#include <cmath>
#include "../utils/constants.h"
#include "../utils/palette.h"
#include "../utils/bitmap.h"
#include "../utils/mandelbrot.h"

#define PALETTE_PATH "palettes/volcano.map"
#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080
#define DEFAULT_FNAME "mandelbrot.bmp"
#define ROOT_PROC 0

using namespace std;

void partition(int num, int div, int* cont, int* displ) {
    int q = num / div;
    int rem = num % div;

    cont[0] = q;
    displ[0] = 0;

    for(int i = 1; i < div; i ++) {
        cont[i] = q;
        if(i == div - 1) {
            cont[i] += rem;
        }
        displ[i] = (q * i) + 1;
    }
}

int main(int argc, char** argv) {
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    int size, rank, count, base, n_pixels;
    int *sub_image, *recv_counts, *displ, *results;
    char* filename = DEFAULT_FNAME;

    if(argc > 1) {
        filename = argv[1];
        if(argc > 2) {
            width = atoi(argv[2]);
            height = atoi(argv[3]);
        }
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double t0 = MPI_Wtime();
    double t1;

    if(rank == ROOT_PROC) {
        cout << "A " << width << "x" << height
             << " mandelbrot image will be generated on file " << filename
             << endl
             << "Please wait, the process may take a while." << endl;
    }

    // When width % number of processors != 0, then the last processor will get
    // slightly more elements
    n_pixels = width * height;
    count = n_pixels / size;
    base = count * rank;
    if(rank == size - 1) {
        count += n_pixels % size;
    }

    // Compute given part of the image. Depends on process rank
    sub_image = partial_mandelbrot(base, count, width, height);

    if(rank == ROOT_PROC) {
        recv_counts = new int[size];
        displ = new int[size];
        results = new int[n_pixels];
        partition(n_pixels, size, recv_counts, displ);
    }

    // Collect results from other processes
    MPI_Gatherv(
        sub_image,
        count,
        MPI_INT,
        results,
        recv_counts,
        displ,
        MPI_INT,
        ROOT_PROC,
        MPI_COMM_WORLD
    );

    /* At this point we have what we need to generate the image */
    t1 = MPI_Wtime();

    delete []sub_image;

    /* Create the image */
    if(rank == ROOT_PROC) {
        Bitmap mandel(width, height);
        Palette palette(MAX_ITER);
        rgb color;

        palette.load_from_file(PALETTE_PATH);

        /* Transform the image from number of iterations to actual pixel values */
        for(int i = 0; i < n_pixels; i++) {
            color = palette[results[i]];
            mandel.set_pixel(i, &color);
        }

        mandel.write_to_file(filename);

        cout << "Successfully wrote " << filename << endl
             << "Process took " << t1 - t0 << " seconds using "
             << size << " process." << endl;

        /* Free memory */
        delete []recv_counts;
        delete []displ;
        delete []results;
    }

    MPI_Finalize();
}
