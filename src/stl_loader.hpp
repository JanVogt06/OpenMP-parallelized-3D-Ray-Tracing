#pragma once
#include <vector>
#include "triangle.hpp"

/* Loads an ASCII STL file. Returns a vector of triangles, all assigned `color`.
 * Returns an empty vector on failure (file not found, read error). */
std::vector<Triangle> load_stl(const char *path, Vec3 color);
