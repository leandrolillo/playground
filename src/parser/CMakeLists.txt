add_library(parser INTERFACE)
target_include_directories(parser INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(parser INTERFACE resources javaLike math)
