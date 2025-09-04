file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB HEADERS CONFIGURE_DEPENDS "*.hpp")
add_library(RayTracingLib ${SOURCES} ${HEADERS})

target_include_directories(RayTracingLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(RayTracingLib PUBLIC ObjectsLib UtilitiesLib)