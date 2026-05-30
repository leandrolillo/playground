add_subdirectory(video)
add_subdirectory(opengl)

target_sources(${TESTS} PUBLIC audioRunnerTests.cpp)
target_sources(${TESTS} PUBLIC physicsRunnerTests.cpp)

target_include_directories(${TESTS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TESTS} PUBLIC audioRunner openGLRunner physicsRunner)