#ifndef STL_LOADER_H
#define STL_LOADER_H

#include <stddef.h>
#include "triangle.h"

/* Loads an ASCII STL file. Allocates an array of triangles, all assigned `color`.
 * Writes the number of triangles to *out_count.
 * Returns NULL on failure (file not found, read error). Caller must free() the result. */
Triangle *load_stl(const char *path, Vec3 color, size_t *out_count);

#endif
