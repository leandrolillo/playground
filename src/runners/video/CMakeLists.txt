#library (videoRunner contains video abstract base class to be inherited by opengl/direct3d in combination with platform implementations)
set(LIBRARY_NAME videoRunner)
add_library(${LIBRARY_NAME} STATIC VideoRunner.cpp)

#sources
add_subdirectory(adapters)
add_subdirectory(camera)
add_subdirectory(renderers)
add_subdirectory(resources)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} PUBLIC playground)

#Dependencies: 
# LibJpeg
find_package(JPEG REQUIRED)
target_include_directories(${LIBRARY_NAME} PUBLIC ${JPEG_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} PUBLIC ${JPEG_LIBRARY})

# LibPng
find_package(PNG REQUIRED)
target_include_directories(${LIBRARY_NAME} PUBLIC ${PNG_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} PUBLIC ${PNG_LIBRARY})

#Freetype
find_package(Freetype REQUIRED)
target_link_libraries(${LIBRARY_NAME} PUBLIC Freetype::Freetype)

#Geometry
FetchContent_Declare(
    geometry
    GIT_REPOSITORY https://github.com/leandrolillo/geometry.git
    GIT_TAG ${GEOMETRY_VERSION}
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(geometry)

target_link_libraries(${LIBRARY_NAME} PUBLIC geometry)
