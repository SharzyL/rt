# Yet Another Raytracer

This is a ray tracer written in 2.3k lines of C++.

Supported features:

- Algorithm
    1. Path tracing
    2. Stochastic progressive photon mapping (SPPM)

- Model
    1. Basic geometry including triangle, plane, sphere
    2. Bézier curve rotating around an axis (solved by Newton's method or triangulation)
    3. Triangle mesh (imported from `.obj` file)

- Material
    1. Lambert reflectance
    2. Speculative material
    3. Reflective and semi-reflective material
    4. Refractive material. Reflection rate calculated by Fresnel-Schlick formula

- Additional features
    1. Texture mapping for mesh, plane, sphere and Bezier surface
    2. Super-sampling for anti-aliasing
    3. Camera with field-of-depth
    4. Intersection finding accelerated by AABB and BVH data structure.
    5. OpenMP multi-threading

## Gallery

1. Reflective and refractive balls:
```shell
./build/RT -i scenes/cornell-2ball.yml -o output/final/cornell-2ball.bmp -p4 -s1024`
```
![](results/cornell-2ball.png)

2. Bezier curve:
```shell
./build/RT -i scenes/cornell-bezier.yml -o output/final/cornell-bezier.bmp -p4 -s512
```
![](results/cornell-bezier.png)

3. Reflective and refractive balls (SPPM version)
```shell
./build/RT_sppm -i scenes/cornell-2ball.yml -o output/final/cornell-2ball-sppm.bmp -p 10000000 -n 50 -r 0.008
```
![](results/cornell-2ball-sppm.png)

4. Color bleeding with path tracing
```shell
./build/RT -i scenes/bleeding.yml -o output/final/bleeding.bmp -p4 -s512
```
![](results/bleeding.png)

5. Depth of field
```shell
./build/RT -i scenes/cornell-depth.yml -o output/final/cornell-depth.bmp -p4 -s1024
```
![](results/cornell-depth.png)

6. Dragon (100k triangles)
```shell
./build/RT -i scenes/dragon.yml -o output/final/dragon.bmp -p4 -s512
```
![](results/dragon.png)

7. M4A1 (100k triangles)
```shell
./build/RT -i scenes/m4a1.yml -o output/final/m4a1.bmp -p4 -s512
```
![](results/m4a1.png)

8. Caustic dragon with SPPM
```shell
./build/RT_sppm -i scenes/caustics.yml -o output/final/caustics.bmp -p 10000000 -n 50 -r 0.008
```
![](results/caustics.png)

## Compilation

Prerequisite:
1. CMake no older than 3.14
2. C++ compiler supporting C++17
3. Network connection to GitHub (for automatic dependency fetching)

After cloning the repository:
```shell
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release --target RT  # compile path tracer
cmake .. -DCMAKE_BUILD_TYPE=Release --target RT_sppm  # compile SPPM
```

Both binary requires a few command line arguments. Run with `--help` to find out.

## External dependencies

1. `glog`: logging
2. `fmt`: string interpolation
3. `tinyobjloader`: Wavefront `.obj` file parser
4. `args`: command line argument parser
5. `yaml-cpp`: yaml parser because we store the scene in a yaml file
6. `lodepng`: PNG encoder/decoder for reading PNG texture
7. `googletest`: tester
