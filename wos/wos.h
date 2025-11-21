#ifndef WOS_H
#define WOS_H

#include <array>
#include <random>

double distance_to_boundary(const std::array<double,3>& x);

double boundary_value(const std::array<double,3>& x);

std::array<double,3> random_point_on_sphere(
    const std::array<double,3>& x0, 
    double r, 
    std::mt19937& rng);

double walk_on_spheres(
    const std::array<double,3>& x0,
    double eps,
    std::mt19937& rng);

#endif