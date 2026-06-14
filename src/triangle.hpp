#pragma once
#include "vec3.hpp"

struct Hit {
    double alpha;
    Vec3   normal;
    Vec3   color;
};

struct Triangle {
    Vec3 T1, T2, T3;
    Vec3 color;

    int intersect(const Vec3& C, const Vec3& D, Hit *out) const;
};
