# src/Objects/CMakeLists.txt

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB HEADERS CONFIGURE_DEPENDS "*.hpp")

add_library(ObjectsLib ${SOURCES} ${HEADERS}
        ../Utilities/Math.hpp)

target_include_directories(ObjectsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(ObjectsLib PUBLIC UtilitiesLib RayTracingLib)