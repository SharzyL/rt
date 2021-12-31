CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(RT CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

include(FetchContent)
FetchContent_Declare(tinyobjloader GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader GIT_TAG v2.0.0rc9)
FetchContent_MakeAvailable(tinyobjloader)
#add_definitions(-DTINYOBJLOADER_IMPLEMENTATION)

add_subdirectory(deps/vecmath)

set(SOURCES
        src/objects/group.cpp
        src/objects/plane.cpp
        src/objects/sphere.cpp
        src/objects/transform.cpp
        src/objects/triangle.cpp
        src/objects/mesh.cpp
        src/hit.cpp
        src/light.cpp
        src/material.cpp
        src/ray.cpp
        src/camera.cpp
        src/image.cpp
        src/main.cpp
        )

find_package(fmt REQUIRED)
find_package(glog REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} vecmath glog::glog fmt tinyobjloader)
target_include_directories(${PROJECT_NAME} PRIVATE src)
