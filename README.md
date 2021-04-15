# Parallel Mandelbrot
A didactical program that I wrote that plots the mandelbrot set by taking advantage of parallelism. Requires [OpenMPI](https://www.open-mpi.org/).

## Instructions
1. Make sure you have the entire OpenMPI ecosystem installed, including `mpirun` and `mpic++`.
2. Clone this repository
3. Run `make ./bin/dynamic` to compile the dynamic version or `make ./bin/static` for the static
   version. 
4. Run the executable either normally (only for the static version for obvious reasons) or with
   `mpirun` to take advantage of parallelism.   

The program also accepts some CLI argument as well, which are autofilled with default values if not
provided. 
```
./bin/dynamic NAME_OF_THE_IMAGE.bmp WIDTH HEIGHT
```

**NOTE**: The programs should be run from the root directory, otherwise the executables won't
recognise the palettes path.

Below are listed two valid commands to run the program:
- `mpirun -np 6 ./bin/static mandel.bmp 2560 1440`: generates the image named `mandel.bmp` with resolution
  2560x1440 using 6 processes. 
- `./bin/static`: generates a full HD image using a single process, without using any form of parallelism
