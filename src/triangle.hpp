#pragma once
#include "vec3.hpp"

struct Hit {
    double alpha;   /* ray parameter at the hit point (C + alpha*D) */
    Vec3   normal;  /* outward normal at the hit */
    Vec3   color;   /* material color of the hit triangle */
};

struct Triangle {
    Vec3 T1, T2, T3;
    Vec3 color;

    /* Ray-triangle intersection via the Möller–Trumbore algorithm.
     * Ray is C + alpha*D (D need not be normalized).
     * On a valid hit, fills *out and returns 1; otherwise returns 0. */
    int intersect(const Vec3& C, const Vec3& D, Hit *out) const;
};
