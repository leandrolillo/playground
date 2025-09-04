target_sources(${TESTS} PUBLIC audioRunnerTests.cpp)
target_sources(${TESTS} PUBLIC videoRunnerTests.cpp)
target_sources(${TESTS} PUBLIC openGLRunnerTests.cpp)
target_sources(${TESTS} PUBLIC physicsRunnerTests.cpp)

target_include_directories(${TESTS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TESTS} PUBLIC audioRunner openGLRunner physicsRunner)