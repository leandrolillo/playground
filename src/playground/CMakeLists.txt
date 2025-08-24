set(LIBRARY_NAME playground)
add_library(${LIBRARY_NAME} INTERFACE)

add_subdirectory(resources)
add_subdirectory(javaLike)
add_subdirectory(core)
add_subdirectory(log)
add_subdirectory(parser)

FetchContent_Declare(
    math
    GIT_REPOSITORY https://github.com/leandrolillo/math.git
    GIT_TAG master
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(math)



# library link dependencies
target_link_libraries(${LIBRARY_NAME} INTERFACE math)

