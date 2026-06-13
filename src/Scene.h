#ifndef OPENMP_PARALLELIZED_3D_RAY_TRACING_SOURCE_H
#define OPENMP_PARALLELIZED_3D_RAY_TRACING_SOURCE_H
#include "triangle.hpp"
#include <vector>


class Scene {
public:
    std::vector<Triangle> triangles;
    void add_triangles(const std::vector<Triangle>& triangles);
    void trace(const Vec3& cameraPos, const Vec3& direction, const Vec3& lightPos, Vec3* color) const;

    void init();
};


#endif //OPENMP_PARALLELIZED_3D_RAY_TRACING_SOURCE_H
