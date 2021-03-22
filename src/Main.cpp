#include <iostream>
#include "bitmap.h"
/* #include <mpi.h> */

int main(int argc, char** argv)
{
    /* int size, rank; */
    /* MPI_Init(&argc, &argv); */
    /* MPI_Comm_size(MPI_COMM_WORLD, &size); */
    /* MPI_Comm_rank(MPI_COMM_WORLD, &rank); */
    /* std::cout << "Hello from process " << rank << std::endl; */
    /* MPI_Finalize(); */

    int width = 560;
    int height = 200;
    Bitmap image(width, height);
    pixel px;
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            px.red = (unsigned char) (i * 255 / width);
            px.green = (unsigned char) (i * 255 / width);
            px.blue = (unsigned char) (i * 255 / width);
            image.set_pixel(i, j, &px);
        }
    }
    image.write_to_file("test.bmp");
}


