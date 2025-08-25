# Source directories
add_subdirectory(Objects)
add_subdirectory(RayTracing)
add_subdirectory(Utilities)

# Main executable
add_executable(RayTracer main.cpp)

# Link all the libraries together
target_link_libraries(RayTracer
    PRIVATE
        ObjectsLib
        RayTracingLib
        UtilitiesLib
)
