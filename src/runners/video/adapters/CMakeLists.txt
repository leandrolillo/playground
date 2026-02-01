add_subdirectory(normalGenerators)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(${LIBRARY_NAME} PUBLIC GeometryResourceAdapter.cpp)