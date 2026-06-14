#pragma once
#include <vector>
#include "triangle.hpp"

std::vector<Triangle> load_stl(const char *path, Vec3 color);
