#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <array>
#include "wos.h"

#ifdef _OPENMP
#include <omp.h>
#endif

// Uniform random point in unit ball
std::array<double,3> random_point_in_ball(std::mt19937 &rng) {
    std::normal_distribution<double> normal(0.0, 1.0);
    std::uniform_real_distribution<double> uni(0.0, 1.0);

    // random direction
    double x = normal(rng);
    double y = normal(rng);
    double z = normal(rng);
    double norm = std::sqrt(x*x + y*y + z*z);
    x /= norm; y /= norm; z /= norm;

    // random radius
    double r = std::cbrt(uni(rng));

    return {r * x, r * y, r * z};
}

int main() {
    int N = 200000;      // samples per point
    int M = 100;         // number of random points to evaluate
    double eps = 1e-3;

    // RNG for generating query points
    std::mt19937 rng(42);

    // -------- generate random points --------
    std::vector<std::array<double,3>> x0_list(M);
    for (int i = 0; i < M; i++)
        x0_list[i] = random_point_in_ball(rng);

    // -------- run WOS on all points --------
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < M; i++) {
        auto &x0 = x0_list[i];
        
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

    // -------- print results --------
    // for (int i = 0; i < M; i++) {
    //     auto &p = x0_list[i];
    //     std::cout << "Point " << i << " = ("
    //               << p[0] << ", "
    //               << p[1] << ", "
    //               << p[2] << ") → "
    //               << approx[i] << "\n";
    // }

    std::cout << "Runtime: " << ms << " ms\n";

    return 0;
}