#pragma once
#include "triangle.hpp"
#include <vector>


class Scene {
public:
    std::vector<Triangle> triangles;
    void add_triangles(const std::vector<Triangle>& triangles);
    void trace(const Vec3& cameraPos, const Vec3& direction, const Vec3& lightPos, Vec3* color) const;

    void init();
};
