CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
PROJECT(RT CXX)

message( ${PROJECT_NAME} " build type: " ${CMAKE_BUILD_TYPE})

# fetch external dependencies
include(FetchContent)
FetchContent_Declare(tinyobjloader GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader GIT_TAG v2.0.0rc9)
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt GIT_TAG 8.0.1)
FetchContent_Declare(glog GIT_REPOSITORY https://github.com/google/glog GIT_TAG v0.5.0)
FetchContent_Declare(args GIT_REPOSITORY https://github.com/Taywee/args GIT_TAG 6.3.0)
FetchContent_Declare(yaml-cpp GIT_REPOSITORY https://github.com/jbeder/yaml-cpp GIT_TAG yaml-cpp-0.7.0)
FetchContent_Declare(lodepng GIT_REPOSITORY https://github.com/lvandeve/lodepng GIT_TAG 8c6a9e30576f07bf470ad6f09458a2dcd7a6a84a)

FetchContent_MakeAvailable(tinyobjloader fmt glog args yaml-cpp lodepng)

# handle lodepng compilation manually
include_directories(${lodepng_SOURCE_DIR})
add_library(lodepng_lib ${lodepng_SOURCE_DIR}/lodepng.cpp)

# local dependency
add_subdirectory(deps/vecmath)

# find openmp from system
find_package(OpenMP)

set(SOURCES
        src/objects/obj_import.cpp
        src/objects/group.cpp
        src/objects/triangle.cpp
        src/objects/mesh.cpp
        src/objects/plane.cpp
        src/objects/sphere.cpp

        src/core/hit.cpp
        src/core/material.cpp
        src/core/ray.cpp
        src/core/camera.cpp
        src/core/renderer.cpp
        src/core/texture.cpp

        src/image.cpp
        src/main.cpp
        src/util.cpp
        src/scene_parser.cpp
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        vecmath
        glog
        fmt
        tinyobjloader
        args
        yaml-cpp
        lodepng_lib
        OpenMP::OpenMP_CXX
        )
target_include_directories(${PROJECT_NAME} PRIVATE src)

add_executable(test_ src/test.cpp)
target_link_libraries(test_ args)
