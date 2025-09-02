FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.10.0
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras) #Enable catch2 / ctest integration

include(Catch)

#executable
set(TESTS "playground_unit_tests")
add_executable(${TESTS})
target_link_libraries(${TESTS} PUBLIC Catch2::Catch2WithMain)

#sources
add_subdirectory(src)

#resources - clean and copy fro scratch
add_custom_command( 
    TARGET ${TESTS} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf
            resources)

add_custom_command( 
    TARGET ${TESTS} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            resources)
            
catch_discover_tests(${TESTS})