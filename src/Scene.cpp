//
// Created by koellman on 13.06.26.
//

#include "Scene.hpp"

void Scene::init() {
    triangles = {};
}

void Scene::add_triangles(const std::vector<Triangle>& newTriangles) {
    for (const auto& triangle : newTriangles) {
        this->triangles.push_back(triangle);
    }
}

void Scene::trace(const Vec3& cameraPos, const Vec3& direction, const Vec3& lightPos, Vec3* color) const {
    Hit closest{};
    bool found = false;

    for (const auto& triangle : triangles) {
        Hit hit{};
        if (triangle.intersect(cameraPos, direction, &hit)) {
            if (!found || hit.alpha < closest.alpha) {
                closest = hit;
                found = true;
            }
        }
    }

    if (!found) {
        *color = {1, 1, 1};
        return;
    }

    const Vec3 hitPoint = vec3_add(cameraPos, vec3_scale(direction, closest.alpha));
    const Vec3 lightDir = vec3_sub(hitPoint, lightPos);
    const Vec3 lightDirNormal = vec3_normalize(lightDir);
    double diffuse = -vec3_dot(closest.normal, lightDirNormal);
    if (diffuse < 0) diffuse = 0;
    *color = vec3_scale(closest.color, diffuse);
}
