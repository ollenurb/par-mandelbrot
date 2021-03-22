#include <iostream>
#include "bitmap.h"
#include <complex>
/* #include <mpi.h> */

#define MANDELBROT_MAX_ITER 1000

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

static unsigned compute_point(double x, double y) {
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

int main(int argc, char** argv)
{
    /* int size, rank; */
    /* MPI_Init(&argc, &argv); */
    /* MPI_Comm_size(MPI_COMM_WORLD, &size); */
    /* MPI_Comm_rank(MPI_COMM_WORLD, &rank); */
    /* std::cout << "Hello from process " << rank << std::endl; */
    /* MPI_Finalize(); */

    int width = 1920;
    int height = 1080;
    Bitmap mandel = mandelbrot(width, height);
    mandel.write_to_file("test.bmp");
}

