#ifndef WOS_H
#define WOS_H

#include <array>
#include <random>

std::array<double,3> random_point_on_sphere(
    const std::array<double,3>& x0, 
    double r, 
    std::mt19937& rng);

double walk_on_spheres(
    const std::array<double,3>& x0,
    double eps,
    std::mt19937& rng,
    std::function<double(const std::array<double,3>&)> sdf,
    std::function<double(const std::array<double,3>&)> boundary_value);

#endif