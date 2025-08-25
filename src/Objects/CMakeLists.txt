# Collect all .cpp files
file(GLOB OBJECTS_SRC "*.cpp")

# Create a library for objects
add_library(ObjectsLib STATIC ${OBJECTS_SRC})

# Include headers
target_include_directories(ObjectsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
