# OpenMP-parallelized 3D Ray Tracer

A simple ray tracer written in C++ that renders 3D scenes from ASCII STL files using the Möller–Trumbore intersection algorithm.

## Building

Compile all `.cpp` files together with your C++17 compiler and OpenMP:

```bash
g++ -std=c++17 -O2 -fopenmp src/triangle.cpp src/stl_loader.cpp main.cpp -o raytracer
```

On macOS with Apple Clang, replace `-fopenmp` with `-Xpreprocessor -fopenmp -lomp`
(requires `brew install libomp`).

## Reference implementation

`given_example/SimpleRenderWithSTL.py` is the provided Python reference implementation.
It shows the complete rendering pipeline (camera setup, ray casting, diffuse shading,
PPM output) and was used as the specification for the C++ port.

## AI notice

The STL loader (`src/stl_loader.hpp` / `src/stl_loader.cpp`) was developed with the
assistance of an AI coding tool (Claude by Anthropic), as permitted by the
project guidelines.

We identified the triangle intersection loop in `Scene::trace` as a parallelization
candidate. Since we were unsure how to implement an OpenMP reduction over a custom
struct, we used AI assistance (Claude by Anthropic) to figure out the syntax and
approach. The implementation was done by us.

However, benchmarking revealed that parallelizing `Scene::trace` with OpenMP is
counterproductive for typical scene sizes. With a 358-triangle STL file and 100,000
trace calls, the results were:

| | Time |
|---|---|
| Without OMP | 212 ms (2.13 µs/trace) |
| With OMP | 1387 ms (13.87 µs/trace) |

The thread management and reduction synchronization overhead outweighs the benefit
at this triangle count. OMP in `trace` would only pay off for scenes with significantly
more triangles. The parallelization was therefore removed from `Scene::trace` and
applied at the render loop level instead (one thread per pixel).

## Render benchmark (render-loop parallelization)

Measured render time (camera + ray casting + shading only; STL loading, PPM
output and PNG conversion are excluded). Scene: `given_example/test.stl`
(358 triangles) at 2000×2000 resolution (4,000,000 rays).

| | Time | Speedup |
|---|---|---|
| Serial (no OpenMP) | 4065 ms | 1.0× |
| Parallel (OpenMP)  | 794 ms  | 5.1× |

The render loop is parallelized at row granularity with
`#pragma omp parallel for schedule(dynamic)`. Each pixel writes to a distinct
index in the output buffer, so no reduction or synchronization is required.
