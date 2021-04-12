#ifndef MANDELBROT_H
#define MANDELBROT_H

/* Compute the mandelbrot set on a given slice of a width*height image.
 * Results are integer values representing how much a given pixel diverges
 * from the orbit.
 * Ex: partial_mandelbrot(0, width*height, width, height) computes the entire
 * image, while partial_mandelbrot(0, 200, width, height) computes the first
 * 200 pixels of the image.
 */
int* partial_mandelbrot(int, int, int, int);

/**
 * Compute a single point on the image.  Given the x and y coordinate of a
 * given pixel, it returns the number of iterations needed to escape the orbit,
 * using an optimized version of the time escape algorithm.  For more info:
 * https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set
 */
int compute_point(double, double);

float scale_interval(float, float, float, float, float);

#endif

