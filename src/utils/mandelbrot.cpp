#include "mandelbrot.h"
#include "constants.h"
#include <assert.h>

int* partial_mandelbrot(int from, int n_pixels, int width, int height) {
    assert(n_pixels <= width*height);
    int* retval = new int[n_pixels];
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


int compute_point(double x0, double y0) {
    int iter = 0;
    double x = 0, y = 0, x2 = 0, y2 = 0;

    while(x2 + y2 <= 4 && iter <= MAX_ITER) {
        y = 2*x*y + y0;
        x = x2 - y2 + x0;
        x2 = x * x;
        y2 = y * y;
        iter += 1;
    }

    return iter;
}

float scale_interval(float a, float b, float c, float d, float t)  {
    return c + (((d - c) * (t - a)) / (b - a));
}
