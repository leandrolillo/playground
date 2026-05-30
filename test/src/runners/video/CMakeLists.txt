target_sources(${TESTS} PUBLIC videoRunnerTests.cpp)
target_sources(${TESTS} PUBLIC renderersTests.cpp)
target_sources(${TESTS} PUBLIC cameraTests.cpp)
target_sources(${TESTS} PUBLIC resourcesTests.cpp)

target_include_directories(${TESTS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(${TESTS} PUBLIC videoRunner)