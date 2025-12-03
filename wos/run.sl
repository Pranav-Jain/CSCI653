#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=8
#SBATCH --time=00:10:00
#SBATCH --output=omp_example.out
#SBATCH -A anakano_429
export OMP_NUM_THREADS=8
echo "Parallel Version"
./wos_omp
echo "Serial Version"
./wos_serial