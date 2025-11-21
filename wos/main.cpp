#include <iostream>
#include <chrono>
#include <random>
#include "wos.h"

#ifdef _OPENMP
#include <omp.h>
#endif

int main() {
    int N = 2000000; // samples
    double eps = 1e-3;
    std::array<double,3> x0 = {0.2, 0.1, 0.0};

    auto start = std::chrono::high_resolution_clock::now();

    double sum = 0.0;

    #pragma omp parallel
    {
        int thread_id = 0;
        #ifdef _OPENMP
            thread_id = omp_get_thread_num();
        #endif
        std::mt19937 rng(1234 + thread_id);

        double local_sum = 0.0;

        #pragma omp for
        for (int i = 0; i < N; i++)
            local_sum += walk_on_spheres(x0, eps, rng);

        #pragma omp atomic
        sum += local_sum;
    }

    double approx = sum / N;

    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Approximate solution: " << approx << "\n";
    std::cout << "Runtime: " << ms << " ms\n";

    return 0;
}