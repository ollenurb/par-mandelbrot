# MP Mandelbrot
A didactical program that I wrote that plots the mandelbrot set by taking advantage of parallelism. Requires [OpenMPI](https://www.open-mpi.org/).

## Instructions
1. Make sure you have the entire OpenMPI ecosystem installed, including `mpirun` and `mpic++`.
2. Clone this repository
3. Build it using `make`
4. Run the executable either normally or with `mpirun` to take advantage of parallelism.   

The program also accepts some CLI argument as well, which are autofilled with default values if not
provided. 
```
main NAME_OF_THE_IMAGE.bmp WIDTH HEIGHT
```

Below are listed two valid commands to run the program:
- `mpirun -np 6 main mandel.bmp 2560 1440`: generates the image named `mandel.bmp` with resolution
  2560x1440 using 6 processes. 
- `main`: generates a full HD image using a single process, without using any form of parallelism
