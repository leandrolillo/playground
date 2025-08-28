add_subdirectory(mock)

#target_sources(${TESTS} PUBLIC playgroundTests.cpp)
target_sources(${TESTS} PUBLIC audioRunnerTests.cpp)
target_sources(${TESTS} PUBLIC videoRunnerTests.cpp)
#target_sources(${TESTS} PUBLIC openGLRunnerTests.cpp)
target_sources(${TESTS} PUBLIC physicsRunnerTests.cpp)

target_include_directories(${TESTS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TESTS} PUBLIC Catch2::Catch2WithMain playground audioRunner openGLRunner physicsRunner)
