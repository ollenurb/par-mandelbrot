#include <iostream>
#include "bitmap.h"
#include <complex>
#include <mpi.h>

#define MANDELBROT_MAX_ITER 500
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
    complex<double> z = 0;
    complex<double> c = x + 1i*y;
    unsigned iter = 0;

    while(abs(z) < 4 && iter < MANDELBROT_MAX_ITER) {
        z = pow(z, 2) + c;
        iter++;
    }

    return iter;
}

Bitmap mandelbrot(unsigned width, unsigned height) {
    Bitmap image(width, height);
    pixel px;
    float x_0, y_0, x, y, xtemp;
    unsigned iter;

    for (unsigned i = 0; i < width; i++) {
        for (unsigned j = 0; j < height; j++) {
            x_0 = scale_interval(0, width, -2.5, 1, i);
            y_0 = scale_interval(0, height, -1, 1, j);
            iter = compute_point(x_0, y_0);
            color_table(iter, &px);
            image.set_pixel(i, j, &px);
        }
    }
    return image;
}

void partition(int num, int div, int* cont, int* displ) {
    int q = num / div;
    int rem = num % div;
    for(int i = 0; i < div; i ++) {
        if(i == div - 1) {
            cont[i] = q + rem;
        }
        else {
            cont[i] = q;
        }
        displ[i] = q * i;
    }
}

int main(int argc, char** argv) {
    int width = 1920;
    int height = 1080;
    int n_pixels = width * height;
    int size, rank;
    int* partitions;
    int* displs;
    pixel* recv_buf;
    Bitmap mandel(width, height);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Datatype mpi_pixel;
    MPI_Type_contiguous(3, MPI_UINT8_T, &mpi_pixel);
    MPI_Type_commit(&mpi_pixel);

    /* Partition data between available processors */
    partitions = (int *) malloc(sizeof(int) * size);
    displs = (int *) malloc(sizeof(int) * size);
    partition(n_pixels, size, partitions, displs);
    recv_buf = (pixel *) malloc(sizeof(pixel) * partitions[rank]);

    MPI_Scatterv(
        mandel.pixels_data.data(),
        partitions,
        displs,
        mpi_pixel,
        recv_buf,
        partitions[rank],
        mpi_pixel,
        ROOT_PROC,
        MPI_COMM_WORLD
    );

    /* Compute mandelbrot on the assigned part */
    int start = displs[rank];
    int img_pos, x, y, x_0, y_0;
    int iter;
    for(int i = 0; i < partitions[rank]; i++) {
        img_pos = start + i;
        x = img_pos % width;
        y = img_pos / width;
        x_0 = scale_interval(0, width, -2.5, 1, x);
        y_0 = scale_interval(0, height, -1, 1, y);
        iter = compute_point(x_0, y_0);
        color_table(iter, &recv_buf[i]);
    }

    MPI_Gatherv(
        recv_buf,
        partitions[rank],
        mpi_pixel,
        mandel.pixels_data.data(),
        partitions,
        displs,
        mpi_pixel,
        ROOT_PROC,
        MPI_COMM_WORLD
    );

    if(rank == ROOT_PROC) {
        mandel.write_to_file("test.bmp");
    }

    free(recv_buf);
    free(partitions);
    free(displs);

    MPI_Finalize();
}


