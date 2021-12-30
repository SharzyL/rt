CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(RT CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

ADD_SUBDIRECTORY(deps/vecmath)

SET(SOURCES
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
        src/scene_parser.cpp)

find_package(fmt REQUIRED)
find_package(glog REQUIRED)

SET(CMAKE_CXX_STANDARD 20)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} vecmath glog::glog fmt)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE src)
