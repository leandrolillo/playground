add_library(log Logger.cpp)
target_include_directories(log INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(log PUBLIC javaLike)