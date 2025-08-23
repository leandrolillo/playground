set(LIBRARY_NAME openGLRunner)
add_library(${LIBRARY_NAME} INTERFACE)

#add_subdirectory(../video)
add_subdirectory(adapters)
target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE playground)


#Dependencies
#this needs to be present in every CmakeList that requires files from playground/cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

#OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${LIBRARY_NAME} INTERFACE ${OPENGL_LIBRARIES})
target_include_directories(${LIBRARY_NAME} INTERFACE ${OPENGL_INCLUDE_DIR})

#SDL2
find_package(SDL2 REQUIRED)
target_include_directories(${LIBRARY_NAME} INTERFACE ${SDL2_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE ${SDL2_LIBRARY})


