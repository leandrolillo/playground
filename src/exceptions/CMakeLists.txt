add_library(exceptions INTERFACE)
target_include_directories(exceptions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(exceptions INTERFACE javaLike)
