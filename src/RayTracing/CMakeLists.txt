add_library(RayTracing
    RayTracing.cpp
    RayTracing.h
)
target_include_directories(RayTracing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(RayTracing PUBLIC SceneObjects VectorUtilities glm)
