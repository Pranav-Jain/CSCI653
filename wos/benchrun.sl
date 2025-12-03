#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=8
#SBATCH --time=00:10:00
#SBATCH --output=omp_bench_example.out
#SBATCH -A anakano_429
export OMP_NUM_THREADS=8
echo "different sample points"
echo "====================="

echo "Parallel Version"
for (( i=1; i < 10; i++ ))
do
  ./wos_omp 2000 $((200 * i)) 0.001 CAPSULE
done
echo "--------------------"
echo "Serial Version"
for (( i=1; i < 10; i++ ))
do
  ./wos_serial 2000 $((200 * i)) 0.001 CAPSULE
done
echo "different walks"
echo "====================="

echo "Parallel Version"
for (( i=1; i < 10; i++ ))
do
  ./wos_omp $((500 * i)) 1000 0.001 CAPSULE
done
echo "--------------------"
echo "Serial Version"
for (( i=1; i < 10; i++ ))
do
  ./wos_serial $((500 * i)) 1000 0.001 CAPSULE
done
