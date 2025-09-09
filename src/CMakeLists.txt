# src/CMakeLists.txt

add_subdirectory(Objects)
add_subdirectory(RayTracing)
add_subdirectory(Utilities)

# Main executable (entry point)
add_executable(RayTracer main.cpp)
target_compile_options(RayTracer PRIVATE -O3 -march=native -ffast-math -fopt-info-vec)


# Link the internal libraries to the app
target_link_libraries(RayTracer
        PRIVATE
        ObjectsLib
        RayTracingLib
        UtilitiesLib
)
