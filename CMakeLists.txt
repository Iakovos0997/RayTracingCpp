cmake_minimum_required(VERSION 3.20)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## Enable optimizations and vectorization
#if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#    add_compile_options(-O3 -march=native -ffast-math -fopt-info-vec)
#elseif (MSVC)
#    add_compile_options(/O2 /arch:AVX2)
#endif()

# ---- External dependencies ----
add_subdirectory(external/eigen)
add_subdirectory(external/glm)
set(INSTALL_GTEST OFF CACHE BOOL "Disable GTest install" FORCE)
add_subdirectory(external/googletest)

# ---- Enable testing ----
include(CTest)
enable_testing()

# ---- Project source + tests ----
add_subdirectory(src)
add_subdirectory(tests)
