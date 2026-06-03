#ifndef INTERSECT_H
#define INTERSECT_H

#include "vec3.h"
#include "triangle.h"

typedef struct {
    double alpha;   /* ray parameter at the hit point (C + alpha*D) */
    Vec3 normal;    /* outward normal at the hit */
    Vec3 color;     /* material color of the hit triangle */
} Hit;

/* Ray-triangle intersection via the Moeller-Trumbore algorithm.
 * Ray is C + alpha*D (D need not be normalized).
 * On a valid hit, fills *out and returns 1; otherwise returns 0. */
int ray_triangle_intersect(Vec3 C, Vec3 D, const Triangle *tri, Hit *out);

#endif
