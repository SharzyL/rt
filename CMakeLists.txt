CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
PROJECT(RT CXX)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

message( ${PROJECT_NAME} " build type: " ${CMAKE_BUILD_TYPE})

# fetch external dependencies
include(FetchContent)
FetchContent_Declare(tinyobjloader GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader GIT_TAG v2.0.0rc9)
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt GIT_TAG 8.0.1)
FetchContent_Declare(glog GIT_REPOSITORY https://github.com/google/glog GIT_TAG v0.5.0)
FetchContent_Declare(args GIT_REPOSITORY https://github.com/Taywee/args GIT_TAG 6.3.0)
FetchContent_Declare(yaml-cpp GIT_REPOSITORY https://github.com/jbeder/yaml-cpp GIT_TAG yaml-cpp-0.7.0)
FetchContent_Declare(lodepng GIT_REPOSITORY https://github.com/lvandeve/lodepng GIT_TAG 8c6a9e30576f07bf470ad6f09458a2dcd7a6a84a)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest GIT_TAG release-1.11.0)

FetchContent_MakeAvailable(tinyobjloader fmt glog args yaml-cpp lodepng googletest)

# handle lodepng compilation manually
add_library(lodepng_lib ${lodepng_SOURCE_DIR}/lodepng.cpp)

# local dependency
add_subdirectory(deps/vecmath)

# find openmp from system
find_package(OpenMP)

set(EXTERNAL_LIBS
        vecmath
        glog
        fmt
        tinyobjloader
        args
        yaml-cpp
        lodepng_lib
        OpenMP::OpenMP_CXX
        )

set(SOURCES
        src/objects/obj_import.cpp
        src/objects/group.cpp
        src/objects/triangle.cpp
        src/objects/mesh.cpp
        src/objects/plane.cpp
        src/objects/sphere.cpp
        src/objects/rotate_bezier.cpp
        src/objects/bvh.cpp

        src/core/hit.cpp
        src/core/material.cpp
        src/core/ray.cpp
        src/core/camera.cpp
        src/core/texture.cpp
        src/core/light.cpp

        src/utils/image.cpp
        src/utils/math_util.cpp
        src/utils/scene_parser.cpp
        src/utils/aabb.cpp
        )

add_executable(${PROJECT_NAME}
        ${SOURCES}
        src/renderers/path_tracing.cpp
        src/pt_main.cpp
        )
target_link_libraries(${PROJECT_NAME} PRIVATE ${EXTERNAL_LIBS})

add_executable(${PROJECT_NAME}_sppm
        ${SOURCES}
        src/renderers/photon_mapping.cpp
        src/sppm_main.cpp
        )
target_link_libraries(${PROJECT_NAME}_sppm PRIVATE ${EXTERNAL_LIBS})

foreach(t IN ITEMS ${PROJECT_NAME} ${PROJECT_NAME}_sppm)
    message("target ${t}")
    target_include_directories(${t} PRIVATE src)
    target_include_directories(${t} PRIVATE ${lodepng_SOURCE_DIR})
    target_compile_features(${t} PRIVATE cxx_std_17)
endforeach()

if (RT_BUILD_TEST)
    # Tests
    add_executable(ball_finder_test
            tests/ball_finder_test.cpp
            src/utils/ball_finder.hpp
            src/utils/math_util.cpp
            )
    target_link_libraries(ball_finder_test PRIVATE ${EXTERNAL_LIBS} gtest_main)

    add_executable(bezier_test
            tests/bezier_intersection_test.cpp
            ${SOURCES}
            )
    target_link_libraries(bezier_test ${EXTERNAL_LIBS} gtest_main)

    include(GoogleTest)
    gtest_discover_tests(ball_finder_test)
endif()
