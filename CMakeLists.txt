CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(RT CXX)

include(FetchContent)
FetchContent_Declare(tinyobjloader GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader GIT_TAG v2.0.0rc9)
FetchContent_MakeAvailable(tinyobjloader)
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt GIT_TAG 8.0.1)
FetchContent_MakeAvailable(fmt)
FetchContent_Declare(glog GIT_REPOSITORY https://github.com/google/glog GIT_TAG v0.5.0)
FetchContent_MakeAvailable(glog)
FetchContent_Declare(args GIT_REPOSITORY https://github.com/Taywee/args GIT_TAG 6.3.0)
FetchContent_MakeAvailable(args)

add_subdirectory(deps/vecmath)

find_package(OpenMP)

set(SOURCES
        src/objects/group.cpp
        src/objects/transform.cpp
        src/objects/triangle.cpp
        src/objects/mesh.cpp
        src/core/hit.cpp
        src/core/material.cpp
        src/core/ray.cpp
        src/core/camera.cpp
        src/core/renderer.cpp
        src/image.cpp
        src/main.cpp
        )

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        vecmath
        glog
        fmt
        tinyobjloader
        args
        OpenMP::OpenMP_CXX
        )
target_include_directories(${PROJECT_NAME} PRIVATE src)

add_executable(test_ src/test.cpp)
target_link_libraries(test_ args)
