#ifndef DOMAIN_H
#define DOMAIN_H

#include <array>
#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>

enum class DomainType { SPHERE, LINK, CAPSULE };

inline DomainType parseDomain(const std::string& s) {
    if (s == "SPHERE")  return DomainType::SPHERE;
    if (s == "LINK")    return DomainType::LINK;
    if (s == "CAPSULE") return DomainType::CAPSULE;

    std::cerr << "Unknown domain type: " << s << "\n";
    std::exit(1);
}

/* -----------------------
      SPHERE DOMAIN
   ----------------------- */
struct SDF_Sphere {
    double operator()(const std::array<double,3>& x) const {
        return 1.0 - std::sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
    }
};
struct Boundary_Sphere {
    double operator()(const std::array<double,3>& x) const {
        return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
    }
};

/* -----------------------
         LINK DOMAIN
   ----------------------- */
struct SDF_Link {
    float le = 1.0f, r1 = 0.5f, r2 = 0.2f;

    double operator()(const std::array<double,3>& p) const {
        double qx = p[0];
        double qy = std::max(std::abs(p[1]) - le, 0.0);
        double qz = p[2];
        double len_xy = std::sqrt(qx*qx + qy*qy);
        return r2 - std::sqrt((len_xy - r1)*(len_xy - r1) + qz*qz);
    }
};
struct Boundary_Link {
    double operator()(const std::array<double,3>& p) const {
        return p[0]*p[0] + p[1]*p[1] + p[2]*p[2];
    }
};

/* -----------------------
       CAPSULE DOMAIN
   ----------------------- */
struct SDF_Capsule {
    std::array<double,3> a{0,0,0};
    std::array<double,3> b{0,2,0};
    double r = 0.5;

    double operator()(const std::array<double,3>& p) const {
        std::array<double,3> pa = { p[0]-a[0], p[1]-a[1], p[2]-a[2] };
        std::array<double,3> ba = { b[0]-a[0], b[1]-a[1], b[2]-a[2] };

        double dot_pa_ba = pa[0]*ba[0] + pa[1]*ba[1] + pa[2]*ba[2];
        double dot_ba_ba = ba[0]*ba[0] + ba[1]*ba[1] + ba[2]*ba[2];
        double h = std::clamp(dot_pa_ba / dot_ba_ba, 0.0, 1.0);

        std::array<double,3> d = {
            pa[0] - ba[0]*h,
            pa[1] - ba[1]*h,
            pa[2] - ba[2]*h
        };

        return r - std::sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);
    }
};
struct Boundary_Capsule {
    double operator()(const std::array<double,3>& p) const {
        return p[0]*p[0] + p[1]*p[1] + p[2]*p[2];
    }
};

/* -----------------------
   Helper to get SDF + boundary
   ----------------------- */

struct DomainFunctions {
    // Function objects
    std::function<double(const std::array<double,3>&)> sdf;
    std::function<double(const std::array<double,3>&)> boundary;
};

inline DomainFunctions getDomainFunctions(DomainType type) {
    switch(type) {
        case DomainType::SPHERE:
            return { SDF_Sphere{}, Boundary_Sphere{} };

        case DomainType::LINK:
            return { SDF_Link{}, Boundary_Link{} };

        case DomainType::CAPSULE:
            return { SDF_Capsule{}, Boundary_Capsule{} };
    }

    // Should never happen
    std::cerr << "Invalid DomainType!\n";
    std::exit(1);
}

#endif
