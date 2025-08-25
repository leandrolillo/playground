FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.5.3
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras) #Enable catch2 / ctest integration

include(Catch)

set(TESTS "playground_unit_tests")

add_executable(${TESTS} playgroundTests.cpp audioRunnerTests.cpp openGLRunnerTests.cpp physicsRunnerTests.cpp)
target_include_directories(${TESTS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TESTS} PUBLIC Catch2::Catch2WithMain playground audioRunner openGLRunner physicsRunner)

catch_discover_tests(${TESTS})