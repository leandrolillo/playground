#Does this include math and geometry as well?
FetchContent_Declare(
    physics
    GIT_REPOSITORY https://github.com/leandrolillo/physics.git
    GIT_TAG master
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(physics)

add_subdirectory(playground)
add_subdirectory(runners)
