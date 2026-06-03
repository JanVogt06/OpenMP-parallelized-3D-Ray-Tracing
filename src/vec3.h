#ifndef VEC3_H
#define VEC3_H

#include <math.h>

typedef struct {
    double x, y, z;
} Vec3;

static inline Vec3 vec3(double x, double y, double z) {
    Vec3 v = {x, y, z};
    return v;
}

static inline Vec3 vec3_add(Vec3 a, Vec3 b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline Vec3 vec3_scale(Vec3 a, double k) {
    return vec3(a.x * k, a.y * k, a.z * k);
}

static inline double vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

static inline double vec3_length(Vec3 a) {
    return sqrt(vec3_dot(a, a));
}

static inline Vec3 vec3_normalize(Vec3 a) {
    double l = vec3_length(a);
    if (l == 0.0) return vec3(0, 0, 0);
    return vec3_scale(a, 1.0 / l);
}

#endif
