# src/CMakeLists.txt

add_subdirectory(Objects)
add_subdirectory(RayTracing)
add_subdirectory(Utilities)

# Main executable (entry point)
add_executable(RayTracer main.cpp)

# Link the internal libraries to the app
target_link_libraries(RayTracer
        PRIVATE
        ObjectsLib
        RayTracingLib
        UtilitiesLib
)
