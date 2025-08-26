target_sources(${TESTS} PUBLIC playgroundTests.cpp audioRunnerTests.cpp videoRunnerTests.cpp openGLRunnerTests.cpp physicsRunnerTests.cpp)
target_include_directories(${TESTS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TESTS} PUBLIC Catch2::Catch2WithMain playground audioRunner openGLRunner physicsRunner)
