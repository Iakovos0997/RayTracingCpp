# tests/CMakeLists.txt

# Example test binary
add_executable(TorusTests TorusTests.cpp
        ../src/Utilities/Math.hpp)

target_link_libraries(TorusTests
        PUBLIC
        ObjectsLib
        RayTracingLib
        UtilitiesLib
        gtest_main
)

# Put the test exe in build/tests
set_target_properties(TorusTests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

include(GoogleTest)
gtest_discover_tests(TorusTests)

gtest_discover_tests(TorusTests
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DISCOVERY_MODE PRE_TEST           # <-- important for IDEs
        DISCOVERY_TIMEOUT 30
)

# Fallback so runners can still launch the whole suite
add_test(NAME TorusTests_all COMMAND $<TARGET_FILE:TorusTests> --gtest_color=yes)

