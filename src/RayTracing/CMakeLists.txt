file(GLOB RAYTRACING_SRC "*.cpp")

add_library(RayTracingLib STATIC ${RAYTRACING_SRC})

target_include_directories(RayTracingLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(RayTracingLib PUBLIC ObjectsLib UtilitiesLib)
