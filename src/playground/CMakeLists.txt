#library
set(LIBRARY_NAME playground)
add_library(${LIBRARY_NAME} INTERFACE)

#sources
add_subdirectory(resources)
add_subdirectory(javaLike)
add_subdirectory(core)
add_subdirectory(log)
add_subdirectory(parser)

target_link_libraries(${LIBRARY_NAME} INTERFACE math)

