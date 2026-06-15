#include "Scene.hpp"
#include "stl_loader.hpp"
#include "write_ppm.hpp"
#include "vec3.hpp"
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

static int clamp_channel(double c) {
    int v = static_cast<int>(255.0 * c);
    if (v < 0)   v = 0;
    if (v > 255) v = 255;
    return v;
}

static void render(int width,
                   int height,
                   const char *stl_file,
                   Vec3 cameraPos,
                   Vec3 lookAt,
                   Vec3 up,
                   Vec3 light) {
    const Vec3 forward   = vec3_normalize(vec3_sub(lookAt, cameraPos));
    const Vec3 right     = vec3_normalize(vec3_cross(forward, up));
    const Vec3 actualUp  = vec3_normalize(vec3_cross(right, forward));
    const double fov      = M_PI / 3.0;
    const double aspect   = static_cast<double>(width) / static_cast<double>(height);
    const double tanHalf  = std::tan(fov / 2.0);

    Scene scene;
    scene.init();
    std::vector<Triangle> tris = load_stl(stl_file, vec3(0.8, 0.8, 0.8));
    scene.add_triangles(tris);
    std::printf("Loaded %zu triangles from STL\n", tris.size());

    std::vector<std::vector<int>> pixels(static_cast<size_t>(width) * height);

    const auto t_start = std::chrono::steady_clock::now();

    int j;
    #pragma omp parallel for default(none) schedule(dynamic) \
        private(j) \
        shared(pixels, scene, cameraPos, light, forward, right, actualUp, \
               width, height, tanHalf, aspect)
    for (j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            double x = (2.0 * (i + 0.5) / (width + 1)  - 1.0) * tanHalf * aspect;
            double y = -(2.0 * (j + 0.5) / (height + 1) - 1.0) * tanHalf;

            Vec3 D = vec3_normalize(
                vec3_add(vec3_add(vec3_scale(right, x), vec3_scale(actualUp, y)), forward));

            Vec3 color{};
            scene.trace(cameraPos, D, light, &color);

            std::vector<int> px(3);
            px[0] = clamp_channel(color.x);
            px[1] = clamp_channel(color.y);
            px[2] = clamp_channel(color.z);
            pixels[static_cast<size_t>(j) * width + i] = px;
        }
    }

    const auto t_end = std::chrono::steady_clock::now();
    const double ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::printf("Render: %.1f ms (%dx%d = %lld rays, %zu triangles)\n",
                ms, width, height,
                static_cast<long long>(width) * height, tris.size());

    std::filesystem::create_directories("../out");
    const std::string ppm = "../out/output.ppm";
    const std::string png = "../out/output.png";
    write_ppm(ppm, width, height, pixels);

    /* Convert the PPM to a PNG via ImageMagick, if available.
     * Tries `magick` (v7) first, then `convert` (v6). */
    if (std::system(("magick " + ppm + " " + png).c_str()) == 0 ||
        std::system(("convert " + ppm + " " + png).c_str()) == 0) {
        std::printf("Wrote %s\n", png.c_str());
    } else {
        std::printf("PNG conversion skipped (ImageMagick not found)\n");
    }
}

int main(int argc, char *argv[]) {
    int width  = argc > 1 ? std::atoi(argv[1]) : 2000;
    int height = argc > 2 ? std::atoi(argv[2]) : 2000;

    const char *stl_file = "../given_example/test.stl";
    const Vec3  cam_pos  = vec3(20, -20, 10);
    const Vec3  lookat   = vec3(0, 0, 3);
    const Vec3  cam_up   = vec3(0, 0, 1);
    const Vec3  light    = vec3(20, -20, 5);

    render(width, height, stl_file, cam_pos, lookat, cam_up, light);
    return 0;
}
