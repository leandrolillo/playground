FetchContent_Declare(
    physics
    GIT_REPOSITORY https://github.com/leandrolillo/physics.git
    GIT_TAG master
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(physics)


add_subdirectory(adapters)
add_subdirectory(camera)
add_subdirectory(renderers)
add_subdirectory(resources)

target_include_directories(video INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_SOURCE_DIR}/adapters ${CMAKE_CURRENT_SOURCE_DIR}/renderers ${CMAKE_CURRENT_SOURCE_DIR}/camera ${CMAKE_CURRENT_SOURCE_DIR}/geometry)
target_link_libraries(${LIBRARY_NAME} INTERFACE playground geometry)

#Dependencies: 
#this needs to be present in every CmakeList that requires files from playground/cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# LibJpeg
find_package(JPEG REQUIRED)
target_link_libraries(${LIBRARY_NAME} INTERFACE ${JPEG_LIBRARY})
target_include_directories(${LIBRARY_NAME} INTERFACE ${JPEG_INCLUDE_DIR})

# LibPng
find_package(PNG REQUIRED)
target_link_libraries(${LIBRARY_NAME} INTERFACE ${PNG_LIBRARY})
target_include_directories(${LIBRARY_NAME} INTERFACE ${PNG_INCLUDE_DIR})
