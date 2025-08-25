cmake_minimum_required(VERSION 3.16)

# Project name and language
project(RayTracer VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add subdirectory for GLM
add_subdirectory(external/glm)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/external/glm)

# Source files
set(SOURCES
    src/main.cpp
    src/SceneObjects.cpp
    src/RayTracing.cpp
)

# Create executable
add_executable(RayTracer ${SOURCES})

# Link GLM (header-only, so optional)
target_link_libraries(RayTracer PUBLIC glm)
