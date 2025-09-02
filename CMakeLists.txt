cmake_minimum_required(VERSION 3.20)
project(RayTracingCpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external/eigen)

# Enable warnings
if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Collect all source files from src
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# Executable from main.cpp + all sources
add_executable(RayTracer ${SOURCES}
        src/Objects/Plane.cpp
        src/Objects/Plane.h
        src/Objects/Cylinder.cpp
        src/Objects/Cylinder.h
        src/Objects/Torus.cpp
        src/Objects/Torus.hpp)

target_link_libraries(RayTracer PRIVATE Eigen3::Eigen)
# Add include directories for headers
target_include_directories(RayTracer PRIVATE
    src/Objects
    src/RayTracing
    src/Utilities
)
