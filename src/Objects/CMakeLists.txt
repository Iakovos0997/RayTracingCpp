add_library(SceneObjects
    SceneObjects.cpp
    SceneObjects.h
)
target_include_directories(SceneObjects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(SceneObjects PUBLIC VectorUtilities glm)
