file(GLOB UTIL_SRC "*.cpp")

add_library(UtilitiesLib STATIC ${UTIL_SRC})

target_include_directories(UtilitiesLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
