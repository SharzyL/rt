# Yet Another Raytracer

This is a ray tracer written by 2.3k lines of C++.

Supported features:

- Algorithm
    1. Path tracing
    2. Stochastic progressive photon mapping (SPPM)

- Model
    1. Basic geometry including triangle, plane, sphere
    2. Bézier curve rotating around an axis
    3. Triangle mesh (imported from `.obj` file)

- Material
    1. Lambert reflectance
    2. Speculative material
    3. Mirror
    4. Refractive material. Reflection rate calculated by Fresnel-Schlick formula

- Additional features
    1. Texture mapping for mesh, plane and sphere
    2. Super-sampling for anti-aliasing
    3. Camera with field-of-depth
    4. Intersection finding accelerated by AABB and BVH data structure.

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
