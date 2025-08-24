#library (videoRunner contains video abstract base class to be inherited by opengl/direct3d in combination with platform implementations)
set(LIBRARY_NAME videoRunner)
add_library(${LIBRARY_NAME} INTERFACE)

#sources
add_subdirectory(adapters)
add_subdirectory(camera)
add_subdirectory(renderers)
add_subdirectory(resources)

target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE playground)

#Dependencies: 
# LibJpeg
find_package(JPEG REQUIRED)
target_link_libraries(${LIBRARY_NAME} INTERFACE ${JPEG_LIBRARY})
target_include_directories(${LIBRARY_NAME} INTERFACE ${JPEG_INCLUDE_DIR})

# LibPng
find_package(PNG REQUIRED)
target_link_libraries(${LIBRARY_NAME} INTERFACE ${PNG_LIBRARY})
target_include_directories(${LIBRARY_NAME} INTERFACE ${PNG_INCLUDE_DIR})
