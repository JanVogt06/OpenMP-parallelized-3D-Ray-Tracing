cmake_minimum_required(VERSION 3.20)
project(RayTracer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_executable(raytracer
    src/main.cpp
    src/stl_loader.cpp
    src/triangle.cpp
        src/Scene.cpp
)

target_include_directories(raytracer PRIVATE src)
target_link_libraries(raytracer PRIVATE OpenMP::OpenMP_CXX)
target_compile_options(raytracer PRIVATE -O2 -Wall)
