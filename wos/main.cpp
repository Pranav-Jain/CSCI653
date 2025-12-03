#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <array>
#include <functional>
#include <algorithm>
#include <fstream>
#include "wos.h"
#include "json.hpp"

#ifdef _OPENMP
#include <omp.h>
#endif

using json = nlohmann::json;

// --- Sample a random point inside a domain using rejection sampling ---
std::array<double,3> random_point_in_domain(
    std::mt19937 &rng,
    const std::array<double,3>& min,
    const std::array<double,3>& max,
    std::function<double(const std::array<double,3>&)> sdf)
{
    std::uniform_real_distribution<double> dist_x(min[0], max[0]);
    std::uniform_real_distribution<double> dist_y(min[1], max[1]);
    std::uniform_real_distribution<double> dist_z(min[2], max[2]);

    std::array<double,3> p;
    while (true) {
        p = { dist_x(rng), dist_y(rng), dist_z(rng) };
        if (sdf(p) > 0) break; // inside domain
    }
    return p;
}

int main() {
    // --- Load config ---
    std::ifstream ifs("config.json");
    if(!ifs.is_open()) {
        std::cerr << "Could not open config.json\n";
        return 1;
    }
    json j;
    ifs >> j;

    int N = j["N"];
    int M = j["M"];
    double eps = j["eps"];
    std::string domain_str = j["domain"];
    int seed = j.value("seed", 42);

    std::mt19937 rng(seed);

    // --- Choose domain type ---
    enum DomainType { SPHERE, LINK, CAPSULE };
    DomainType domain = LINK;
    if(domain_str == "SPHERE") domain = SPHERE;
    else if(domain_str == "LINK") domain = LINK;
    else if(domain_str == "CAPSULE") domain = CAPSULE;
    else {
        std::cerr << "Unknown domain type: " << domain_str << "\n";
        return 1;
    }

    // --- Define SDFs and boundary values ---

    // Sphere
    auto sdf_sphere = [](const std::array<double,3>& x) -> double {
        return 1.0 - std::sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
    };
    auto boundary_sphere = [](const std::array<double,3>& x) -> double {
        return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
    };

    // Link
    float le = 1.0f, r1 = 0.5f, r2 = 0.2f;
    auto sdf_link = [=](const std::array<double,3>& p) -> double {
        double qx = p[0];
        double qy = std::max(std::abs(p[1]) - le, 0.0);
        double qz = p[2];
        double len_xy = std::sqrt(qx*qx + qy*qy);
        return r2 - std::sqrt((len_xy - r1)*(len_xy - r1) + qz*qz);
    };
    auto boundary_link = [](const std::array<double,3>& p) -> double {
        return p[0]*p[0] + p[1]*p[1] + p[2]*p[2];
    };

    // Capsule
    std::array<double,3> a = {0,0,0};
    std::array<double,3> b = {0,2,0};
    double capsule_r = 0.5;
    auto sdf_capsule = [=](const std::array<double,3>& p) -> double {
        std::array<double,3> pa = { p[0]-a[0], p[1]-a[1], p[2]-a[2] };
        std::array<double,3> ba = { b[0]-a[0], b[1]-a[1], b[2]-a[2] };
        double dot_pa_ba = pa[0]*ba[0] + pa[1]*ba[1] + pa[2]*ba[2];
        double dot_ba_ba = ba[0]*ba[0] + ba[1]*ba[1] + ba[2]*ba[2];
        double h = std::clamp(dot_pa_ba / dot_ba_ba, 0.0, 1.0);
        std::array<double,3> diff = { pa[0]-ba[0]*h, pa[1]-ba[1]*h, pa[2]-ba[2]*h };
        return capsule_r - std::sqrt(diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2]);
    };
    auto boundary_capsule = [](const std::array<double,3>& p) -> double {
        return p[0]*p[0] + p[1]*p[1] + p[2]*p[2];
    };

    // --- Define bounding box for rejection sampling ---
    std::array<double,3> bbox_min = {-2,-2,-2};
    std::array<double,3> bbox_max = { 2, 2, 2};

    // --- Generate random points inside domain ---
    std::vector<std::array<double,3>> x0_list(M);
    for(int i=0; i<M; i++) {
        if(domain == SPHERE)
            x0_list[i] = random_point_in_domain(rng, bbox_min, bbox_max, sdf_sphere);
        else if(domain == LINK)
            x0_list[i] = random_point_in_domain(rng, bbox_min, bbox_max, sdf_link);
        else
            x0_list[i] = random_point_in_domain(rng, bbox_min, bbox_max, sdf_capsule);
    }

    // --- Run Walk-on-Spheres ---
    std::vector<double> results(M,0.0);
    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        int tid = 0;
        #ifdef _OPENMP
            tid = omp_get_thread_num();
        #endif
        std::mt19937 rng_local(1234 + 17*tid);

        #pragma omp for collapse(2) schedule(static)
        for(int p=0; p<M; p++) {
            for(int s=0; s<N; s++) {
                double val;
                if(domain == SPHERE)
                    val = walk_on_spheres(x0_list[p], eps, rng_local, sdf_sphere, boundary_sphere);
                else if(domain == LINK)
                    val = walk_on_spheres(x0_list[p], eps, rng_local, sdf_link, boundary_link);
                else
                    val = walk_on_spheres(x0_list[p], eps, rng_local, sdf_capsule, boundary_capsule);

                #pragma omp critical
                results[p] += val;
            }
        }
    }

    for(int p=0; p<M; p++)
        results[p] /= N;

    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double,std::milli>(end-start).count();

    std::cout << "Runtime: " << ms << " ms\n";

    // Optional: print results
    /*
    for(int p=0; p<M; p++) {
        auto &pt = x0_list[p];
        std::cout << "Point " << p << " (" << pt[0] << "," << pt[1] << "," << pt[2]
                  << ") -> " << results[p] << "\n";
    }
    */

    return 0;
}