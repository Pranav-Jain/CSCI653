#include "wos.h"
#include <cmath>

double distance_to_boundary(const std::array<double,3>& x) {
    double r = std::sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
    return 1.0 - r;
}

double boundary_value(const std::array<double,3>& x) {
    // Example Dirichlet boundary condition
    return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
}

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

double walk_on_spheres(
        const std::array<double,3>& x0,
        double eps,
        std::mt19937& rng)
{
    std::array<double,3> x = x0;

    while (true) {
        double d = distance_to_boundary(x);
        if (d < eps) {
            // Project to boundary of unit sphere
            double r = std::sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
            x = { x[0]/r, x[1]/r, x[2]/r };
            return boundary_value(x);
        }

        x = random_point_on_sphere(x, d, rng);
    }
}