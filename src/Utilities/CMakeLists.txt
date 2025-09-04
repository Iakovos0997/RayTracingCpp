file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB HEADERS CONFIGURE_DEPENDS "*.hpp")
add_library(UtilitiesLib ${SOURCES} ${HEADERS})

target_include_directories(UtilitiesLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(UtilitiesLib PUBLIC Eigen3::Eigen)