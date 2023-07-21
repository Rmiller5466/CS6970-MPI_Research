#!/bin/bash

#SBATCH -N 2
#SBATCH -n 4
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=6
#SBATCH --job-name=ClearedEnv
#SBATCH --output=output_Two.txt

module purge

# Module for MPICH:
module load mpich/3.3.2-gcc-8.5.0

# Module for OpenMPI:
# module load openmpi/4.0.3-gcc-8.5.0

# Total iterations of 5 hundred million - 70 Seconds
# Total iterations of 1 hundred million - 15 Seconds
mpirun -np 4 ./Parent.mpi 5 100000000