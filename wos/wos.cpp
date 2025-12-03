#include <array>
#include <cmath>
#include <random>
#include <functional>
#include <algorithm> // for std::max, std::clamp

// Generate a random point on a sphere of radius r around x0
std::array<double,3> random_point_on_sphere(const std::array<double,3>& x0, double r, std::mt19937& rng)
{
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    double x, y, z, norm;
    do {
        x = dist(rng);
        y = dist(rng);
        z = dist(rng);
        norm = std::sqrt(x*x + y*y + z*z);
    } while (norm == 0.0 || norm > 1.0);

    return { x0[0] + r*(x/norm),
             x0[1] + r*(y/norm),
             x0[2] + r*(z/norm) };
}

// Generic Walk-on-Spheres solver
double walk_on_spheres(
        const std::array<double,3>& x0,
        double eps,
        std::mt19937& rng,
        std::function<double(const std::array<double,3>&)> sdf,
        std::function<double(const std::array<double,3>&)> boundary_value)
{
    std::array<double,3> x = x0;

    while (true) {
        double d = sdf(x);  // distance to boundary
        if (d < eps) {
            return boundary_value(x);
        }
        x = random_point_on_sphere(x, d, rng);
    }
}