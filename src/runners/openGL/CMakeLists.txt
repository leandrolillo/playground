#library
set(LIBRARY_NAME openGLRunner)
add_library(${LIBRARY_NAME} INTERFACE)

#sources
add_subdirectory(adapters)
add_subdirectory(renderers)
target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE playground videoRunner)


#Dependencies
#OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(${LIBRARY_NAME} INTERFACE ${OPENGL_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE ${OPENGL_LIBRARIES})

#SDL
FetchContent_Declare(
    SDL
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG ${SDL_VERSION}
    GIT_SHALLOW TRUE
    OVERRIDE_FIND_PACKAGE)    
    
set(BUILD_SHARED_LIBS FALSE)
set(SDL_AUDIO            OFF)
set(SDL_VIDEO            ON)
set(SDL_RENDER           OFF)
set(SDL_RENDER_GPU       OFF)
set(SDL_GPU              OFF)
set(SDL_OPENGL           ON)
set(SDL_CAMERA           OFF)
set(SDL_JOYSTICK         ON)
set(SDL_VIRTUAL_JOYSTICK OFF)
set(SDL_HAPTIC           OFF)
set(SDL_HIDAPI           OFF)
set(SDL_POWER            OFF)
set(SDL_SENSOR           OFF)
set(SDL_DIALOG           OFF)
set(SDL_DISKAUDIO        OFF)
set(SDL_DUMMYAUDIO       OFF)
set(SDL_DUMMYCAMERA      OFF)
set(SDL_DUMMYVIDEO       OFF)
set(SDL_OFFSCREEN        OFF)

FetchContent_MakeAvailable(SDL)

#target_include_directories(${LIBRARY_NAME} INTERFACE ${SDL2_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE SDL3::SDL3)
