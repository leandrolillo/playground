add_library(test INTERFACE)
target_include_directories(test INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# library link dependencies
target_link_libraries(test INTERFACE core math)
