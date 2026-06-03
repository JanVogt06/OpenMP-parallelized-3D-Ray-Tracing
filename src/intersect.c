#include "intersect.h"

int ray_triangle_intersect(Vec3 C, Vec3 D, const Triangle *tri, Hit *out) {
    const double eps = 1e-6;

    Vec3 E1 = vec3_sub(tri->T2, tri->T1);
    Vec3 E2 = vec3_sub(tri->T3, tri->T1);
    Vec3 U  = vec3_cross(D, E2);
    double beta = vec3_dot(E1, U);

    /* ray parallel to triangle */
    if (beta > -eps && beta < eps)
        return 0;

    double beta_inv = 1.0 / beta;
    Vec3 V = vec3_sub(C, tri->T1);

    double lambda2 = vec3_dot(V, U) * beta_inv;
    if (lambda2 < 0.0 || lambda2 > 1.0)
        return 0;

    double lambda3 = vec3_dot(D, vec3_cross(V, E1)) * beta_inv;
    if (lambda3 < 0.0 || lambda2 + lambda3 > 1.0)
        return 0;

    double alpha = vec3_dot(E2, vec3_cross(V, E1)) * beta_inv;
    if (alpha <= eps)
        return 0;

    out->alpha  = alpha;
    out->normal = vec3_normalize(vec3_cross(E1, E2));
    out->color  = tri->color;
    return 1;
}
