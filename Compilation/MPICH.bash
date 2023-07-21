#!/usr/bin/bash

# compile the source
/opt/apps/mpi/mpich-3.3.2_gcc-8.5.0/bin/mpicc -o $1.mpi $1.c -lm

# make it executable
chmod 700 $1.mpi

