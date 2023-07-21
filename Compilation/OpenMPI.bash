#!/usr/bin/bash

# compile the source
/opt/apps/mpi/openmpi-4.0.3_gcc-8.5.0/bin/mpicc -o $1.mpi $1.c -lm

# make it executable
chmod 700 $1.mpi

