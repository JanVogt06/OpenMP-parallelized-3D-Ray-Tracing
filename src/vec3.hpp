#pragma once
#include <cmath>

struct Vec3 {
    double x, y, z;
};

inline Vec3 vec3(double x, double y, double z) { return {x, y, z}; }

inline Vec3 vec3_add(Vec3 a, Vec3 b)   { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
inline Vec3 vec3_sub(Vec3 a, Vec3 b)   { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
inline Vec3 vec3_scale(Vec3 a, double k) { return {a.x*k, a.y*k, a.z*k}; }
inline Vec3 vec3_multiply(Vec3 a, Vec3 b) { return {a.x*b.x, a.y*b.y, a.z*b.z}; }

inline double vec3_dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

inline double vec3_length(Vec3 a) { return std::sqrt(vec3_dot(a, a)); }

inline Vec3 vec3_normalize(Vec3 a) {
    double l = vec3_length(a);
    if (l == 0.0) return {0, 0, 0};
    return vec3_scale(a, 1.0 / l);
}
