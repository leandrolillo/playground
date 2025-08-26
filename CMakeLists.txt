#cmake/compiler options
#add_definitions(-w) # hide warnings
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_FIND_FRAMEWORK LAST)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target)

include(FetchContent) #Enable FetchContent for cmake git dependencies

#Project
project(playground)

#source code
add_subdirectory(src)

#tests
include(CTest) #Enable CTest - unfortunately this has to be on the main CMakeLists file or it will not detect tests.
enable_testing()
add_subdirectory(test)