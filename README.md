# OpenMP 3D Ray Tracer

C++-Raytracer, der Szenen aus ASCII-STL-Dateien rendert. Schnittberechnung über
Möller–Trumbore, parallelisiert pro Pixel mit OpenMP.

## Build

### Linux (GCC oder Clang)
```bash
cmake -S . -B build
cmake --build build
./build/raytracer
```

### Windows (MSVC)
```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\raytracer.exe
```

### macOS (Apple Clang + libomp)
Benötigt `brew install libomp`:
```bash
cmake -S . -B build \
  -DOpenMP_CXX_FLAGS="-Xpreprocessor -fopenmp -I$(brew --prefix libomp)/include" \
  -DOpenMP_CXX_LIB_NAMES="omp" \
  -DOpenMP_omp_LIBRARY="$(brew --prefix libomp)/lib/libomp.dylib"
cmake --build build
./build/raytracer
```

Optionale Auflösung: `./raytracer [width] [height]`.
Referenzimplementierung: `given_example/SimpleRenderWithSTL.py`.

## KI-Hinweis
Der STL-Loader (`src/stl_loader.hpp` / `.cpp`) und die OpenMP-Reduktions-Syntax für die
Dreiecksschleife in `Scene::trace` wurden mit KI-Unterstützung (Claude, Anthropic)
entwickelt, wie laut Projektvorgaben erlaubt. Die Implementierung stammt von uns.

## Parallelisierung von `Scene::trace`
Eine OpenMP-Reduktion über die Dreiecksschleife in `Scene::trace` haben wir getestet,
sie ist für übliche Szenen aber langsamer (358 Dreiecke, 100 000 Trace-Aufrufe):

| Variante    | Zeit pro Trace |
|-------------|---------------:|
| ohne OMP    |       2.13 µs  |
| mit OMP     |      13.87 µs  |

Thread- und Synchronisationsoverhead überwiegen bei dieser Dreieckszahl: Die parallele
Region wird bei jedem der 100 000 Trace-Aufrufe neu betreten, das Fork/Join der Threads
und die Reduktion über die eigene Treffer-Struct kosten also pro Aufruf mehr Zeit, als die
358 sehr günstigen Möller–Trumbore-Tests selbst brauchen. Wir parallelisieren daher die
Render-Schleife (ein Thread pro Pixel) statt `trace`. Da jeder
Pixel an einen eigenen Index schreibt, ist die PPM-Ausgabe für p=1 und p=16 byteweise
identisch.

## Render-Benchmark (Aufgabe 7.2)
AMD Ryzen 9 7900 (12 Kerne / 24 Threads). Szene: `given_example/test.stl` (358 Dreiecke).
Gemessen nur die Render-Schleife (ohne STL-Laden, PPM-Ausgabe, PNG-Konvertierung),
Median aus 3 Läufen. S(p) = T(1)/T(p), E(p) = S(p)/p.

### 512 × 512
| p  | T(p) [ms] |   S(p) |  E(p) |
|----|----------:|-------:|------:|
|  1 |     567.2 |  1.000 | 1.000 |
|  2 |     285.9 |  1.984 | 0.992 |
|  4 |     144.4 |  3.928 | 0.982 |
|  8 |      73.5 |  7.717 | 0.965 |
| 16 |      49.2 | 11.528 | 0.721 |
| 24 |      50.8 | 11.165 | 0.465 |

### 1024 × 1024
| p  | T(p) [ms] |   S(p) |  E(p) |
|----|----------:|-------:|------:|
|  1 |    2223.8 |  1.000 | 1.000 |
|  2 |    1121.0 |  1.984 | 0.992 |
|  4 |     564.4 |  3.940 | 0.985 |
|  8 |     286.4 |  7.765 | 0.971 |
| 16 |     187.8 | 11.841 | 0.740 |
| 24 |     177.1 | 12.557 | 0.523 |

### 2048 × 2048
| p  | T(p) [ms] |   S(p) |  E(p) |
|----|----------:|-------:|------:|
|  1 |    8869.6 |  1.000 | 1.000 |
|  2 |    4467.7 |  1.985 | 0.993 |
|  4 |    2306.6 |  3.845 | 0.961 |
|  8 |    1140.3 |  7.778 | 0.972 |
| 16 |     759.6 | 11.677 | 0.730 |
| 24 |     713.9 | 12.424 | 0.518 |

### Diskussion
![Speedup-Plot](speedup_plot.png)

Bis p = 8 (ein Thread pro physischem Kern) folgt der Speedup nahezu der Ideallinie.
Danach flacht er ab: Die Threads 9–24 teilen sich Kerne per SMT und konkurrieren um
dieselben FPUs, und bei höheren Auflösungen passt der Pixelpuffer nicht mehr in den
L3-Cache, sodass die Schreibbandbreite limitiert.

Ein Amdahl-Fit `S(p) = 1 / (f + (1 − f)/p)` ergibt einen seriellen Anteil von etwa 3–4 %
(Kamera-Setup, Pufferallokation, Thread-Start):

| Auflösung   | f      | S_max = 1/f |
|-------------|-------:|------------:|
| 512 × 512   | 0.0401 |      ~24.9  |
| 1024 × 1024 | 0.0335 |      ~29.8  |
| 2048 × 2048 | 0.0344 |      ~29.1  |

Das theoretische S_max wird nicht erreicht, da die 12 physischen Kerne den praktischen
Speedup ohnehin bei rund 12× begrenzen.