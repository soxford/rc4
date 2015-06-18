#!/bin/bash

# Pass environment to compute nodes
#  * nothing special needed, slurm propagates the environment by default

# Give the job a name
#SBATCH --job-name=strmcllct

# Walltime limit
#SBATCH --time=00:10:00

# Use 2 nodes, each running 16 processes
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16

# Email me at the beginning, end of job and on abortion
#SBATCH --mail-type=ALL

# at the following address
#SBATCH --mail-user=simon.campbell@cs.ox.ac.uk

# Go to the directory where you submitted the job
#  * nothing special needed, slurm goes there by default

### script to define MPI_HOSTS for mpirun -- uncomment as appropriate
## Arcus-b: source the script enable_arcus-b_mpi.sh
. enable_arcus-b_mpi.sh

# Launch the application
mpirun $MPI_HOSTS ./controlTest
