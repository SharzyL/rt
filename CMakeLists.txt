CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
PROJECT(RT CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

ADD_SUBDIRECTORY(deps/vecmath)

SET(SOURCES
        src/image.cpp
        src/main.cpp
        src/objects/mesh.cpp
        src/scene_parser.cpp)

find_package(fmt REQUIRED)
find_package(glog REQUIRED)

SET(CMAKE_CXX_STANDARD 20)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} vecmath glog::glog fmt)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)
