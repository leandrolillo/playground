#Libraries
set(LIBRARY_NAME physicsRunner)
add_library(${LIBRARY_NAME} INTERFACE)

#sources
target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#dependencies
FetchContent_Declare(
    physics
    GIT_REPOSITORY https://github.com/leandrolillo/physics.git
    GIT_TAG master
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(physics)

target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE playground physics)