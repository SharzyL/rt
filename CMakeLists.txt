CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(RT CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

include(FetchContent)
FetchContent_Declare(tinyobjloader GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader GIT_TAG v2.0.0rc9)
FetchContent_MakeAvailable(tinyobjloader)

ADD_SUBDIRECTORY(deps/vecmath)

SET(SOURCES
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

SET(CMAKE_CXX_STANDARD 20)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} vecmath glog::glog fmt tinyobjloader)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE src)
