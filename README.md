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
