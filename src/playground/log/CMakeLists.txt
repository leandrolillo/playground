target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(${LIBRARY_NAME} PUBLIC logger.cpp)