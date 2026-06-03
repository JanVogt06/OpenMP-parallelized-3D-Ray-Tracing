#include "stl_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Triangle *load_stl(const char *path, Vec3 color, size_t *out_count) {
    FILE *f = fopen(path, "r");
    if (!f) {
        *out_count = 0;
        return NULL;
    }

    size_t cap = 256;
    size_t n = 0;
    Triangle *tris = malloc(cap * sizeof(Triangle));
    if (!tris) { fclose(f); *out_count = 0; return NULL; }

    Vec3 verts[3];
    int vcount = 0;
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
            if (n == cap) {
                cap *= 2;
                Triangle *grown = realloc(tris, cap * sizeof(Triangle));
                if (!grown) { free(tris); fclose(f); *out_count = 0; return NULL; }
                tris = grown;
            }
            tris[n].T1 = verts[0];
            tris[n].T2 = verts[1];
            tris[n].T3 = verts[2];
            tris[n].color = color;
            n++;
            vcount = 0;
        }
    }

    fclose(f);
    *out_count = n;
    return tris;
}
