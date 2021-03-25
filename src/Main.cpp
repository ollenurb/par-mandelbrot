#include <iostream>
#include "bitmap.h"
#include <complex>
#include <mpi.h>
#include <assert.h>

#define MANDELBROT_MAX_ITER 700
#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080
#define DEFAULT_FNAME "mandelbrot.bmp"
#define ROOT_PROC 0

using namespace std;

float scale_interval(float a, float b, float c, float d, float t)  {
    return c + (((d - c) * (t - a)) / (b - a));
}

void color_table(unsigned iter, pixel* px) {
    unsigned val = static_cast<unsigned>(scale_interval(0, MANDELBROT_MAX_ITER, 0, 255, iter));
    px->red = val;
    px->green = val;
    px->blue = val;
}

static int compute_point(double x, double y) {
    complex<double> z(0);
    complex<double> c(x, y);
    unsigned iter = 0;

    while(abs(z) < 4 && iter < MANDELBROT_MAX_ITER) {
        z = pow(z, 2) + c;
        iter++;
    }

    return iter;
}

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

/* Compute the mandelbrot set on a given slice of a width*height image.
 * Results are integer values representing how much a given pixel diverges
 * from the orbit.
 * Ex: partial_mandelbrot(0, width*height, width, height) computes the entire
 * image, while partial_mandelbrot(0, 200, width, height) computes the first
 * 200 pixels of the image.
 */
int* partial_mandelbrot(int from, int n_pixels, int width, int height) {
    assert(n_pixels <= width*height);
    int* retval = (int*) malloc(sizeof(int) * n_pixels);
    int x, y;
    double x_0, y_0;

    for(int i = 0; i < n_pixels; i++) {
        x = (from + i) % width;
        y = (from + i) / width;
        x_0 = scale_interval(0, width, -2.5, 1, x);
        y_0 = scale_interval(0, height, -1, 1, y);
        retval[i] = compute_point(x_0, y_0);
    }
    return retval;
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
             << endl ;
        cout << "Please wait, the process may take a while." << endl;
    }

    /* When width % number of processors != 0, then the last processor will get
     * slightly more elements
     */
    n_pixels = width * height;
    count = n_pixels / size;
    base = count * rank;
    if(rank == size - 1) {
        count += n_pixels % size;
    }

    /* Compute given part of the image. Depends on process rank */
    sub_image = partial_mandelbrot(base, count, width, height);

    if(rank == ROOT_PROC) {
        recv_counts = (int*) malloc(sizeof(int) * size);
        displ = (int*) malloc(sizeof(int) * size);
        results = (int*) malloc(sizeof(int) * n_pixels);
        partition(n_pixels, size, recv_counts, displ);
    }

    /* Collect results from other processes */
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

    free(sub_image);

    if(rank == ROOT_PROC) {
        Bitmap mandel(width, height);
        pixel px;
        /* Apply RGB values based on iteration values for each pixel */
        for(int i = 0; i < n_pixels; i++) {
            color_table(results[i], &px);
            mandel.set_pixel(i, &px);
        }

        mandel.write_to_file(filename);
        t1 = MPI_Wtime();
        cout << "Successfully wrote " << filename << endl
             << "Process took " << t1 - t0 << " seconds using "
             << size << " process." << endl;

        free(recv_counts);
        free(displ);
        free(results);
    }

    MPI_Finalize();
}
