#include "stl_loader.hpp"
#include <cstdio>
#include <cstring>

std::vector<Triangle> load_stl(const char *path, Vec3 color) {
    std::vector<Triangle> tris;

    FILE *f = fopen(path, "r");
    if (!f) return tris;

    Vec3 verts[3];
    int  vcount = 0;
    char line[512];

    while (fgets(line, sizeof(line), f)) {
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (strncmp(p, "vertex", 6) != 0) continue;
        p += 6;

        double x, y, z;
        if (sscanf(p, "%lf %lf %lf", &x, &y, &z) != 3) continue;
        verts[vcount++] = vec3(x, y, z);

        if (vcount == 3) {
            Triangle tri;
            tri.T1    = verts[0];
            tri.T2    = verts[1];
            tri.T3    = verts[2];
            tri.color = color;
            tris.push_back(tri);
            vcount = 0;
        }
    }

    fclose(f);
    return tris;
}
