add_executable(${LIBRARY_NAME} src/Main.cpp)

FetchContent_Declare(
    physics
    GIT_REPOSITORY https://github.com/leandrolillo/physics.git
    GIT_TAG master
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(physics)

add_subdirectory(core)
add_subdirectory(video)
add_subdirectory(audio)
add_subdirectory(openGL)


# library link dependencies
target_link_libraries(${LIBRARY_NAME} physics core openGL audio)
