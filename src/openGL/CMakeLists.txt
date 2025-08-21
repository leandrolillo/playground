add_library(openGL INTERFACE)
target_include_directories(openGL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(openGL INTERFACE ${OPENGL_LIBRARIES})
target_include_directories(openGL INTERFACE ${OPENGL_INCLUDE_DIR})

#this needs to be present in every CmakeList that requires files from playground/cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

#SDL2
find_package(SDL2 REQUIRED)
target_include_directories(openGL INTERFACE ${SDL2_INCLUDE_DIR})
target_link_libraries(openGL INTERFACE ${SDL2_LIBRARY})


# library link dependencies
target_link_libraries(openGL INTERFACE core parser video math)
