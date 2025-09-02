target_sources(${TESTS} PUBLIC playgroundTests.cpp ResourceManagerTest.cpp PathsTest.cpp)

target_include_directories(${TESTS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TESTS} PUBLIC playground)