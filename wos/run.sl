#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=8
#SBATCH --time=00:10:00
#SBATCH --output=omp_run_example.out
#SBATCH -A anakano_429
export OMP_NUM_THREADS=8
echo "Parallel Version"
./wos_omp 2000 100 0.0001 LINK

echo "Serial Version"
./wos_serial 2000 100 0.0001 LINK