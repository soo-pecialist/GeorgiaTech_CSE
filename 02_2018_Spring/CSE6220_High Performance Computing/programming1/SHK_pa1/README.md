CX 4220 / CSE 6220 Programming Assignment 1
===========================================

## Code hosting

You are highly encouraged to use a code version management tool such as git.
This will help you to code in a team and keep track of your progress.

However, do not upload your code to public repositories. If you want to use
version management for collaboration, make sure to use private repositories.

I highly recommend using Georgia Tech's Enterprise Github installation at
https://github.gatech.edu/

We will also be hosting the framework code for the programming assignment on
there.  If you find any issues with the code framework and we have to make
changes, we will publish those changes in that GitHub repository additionally to
sending out the updated framework.

## Code structure

All the code is located at the root level of the project.

There are multiple header and .c files, your implementation will go
into the following files:

- `score_analyser.c`: Implement the sequential algorithm for solving this assignment
  according to the function declarations in `score_analyser.h`
- `mpi_score_analyser.c`: Implement the parallel algorithm according to the
  declarations in `mpi_score_analyser.h`.
- `utils.h` and `utils.cpp`: Implement common utility functions in these files


Other files containing code that you should not change are:
- `score_analyser.h`
- `mpi_score_analyser.h`
- `main.cpp`: Implements code for the main executable `jacobi`. This does
  input/output reading and calling of the actual functions.
- `io.h`: implements IO functions and random input generation
- `mpi_gtest.cpp`: MPI wrapper for the GTest framework

Utility scripts (you may play around with these to generate your own custom
input):
- `generate_input.py`: Python script to generate inputs. You can modify this
  code to generate different inputs.
  
## Compiling

In order to compile everything, simply run
```sh
$ make all
```
## Executing

```sh
$ mpirun -np 4 ./grader_cal -f scores_100.txt
$ mpirun -np 4 ./grader_cal -n 100
```

## Running Tests

For running all tests do:
```sh
$ make test
```

You can also run the two tests (sequential/MPI) separately by either:
```sh
$ ./seq_tests
```
or
```sh
$ mpirun -np 4 ./mpi_tests
```


