#Library
set(LIBRARY_NAME audioRunner)
add_library(${LIBRARY_NAME} INTERFACE)

#sources
add_subdirectory(resources)
add_subdirectory(adapters)

target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE playground)

#dependencies
#ogg
FetchContent_Declare(
    Ogg
    GIT_REPOSITORY https://gitlab.xiph.org/xiph/ogg.git
    GIT_TAG v1.3.6
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(Ogg)
target_link_libraries(${LIBRARY_NAME} INTERFACE Ogg::ogg)

#vorbis
FetchContent_Declare(
    Vorbis
    GIT_REPOSITORY https://gitlab.xiph.org/xiph/vorbis.git
    GIT_TAG v1.3.7
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(Vorbis)
target_link_libraries(${LIBRARY_NAME} INTERFACE vorbis vorbisfile)

#openAL
find_package(OpenAL REQUIRED)
target_include_directories(${LIBRARY_NAME} INTERFACE ${OPENAL_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE ${OPENAL_LIBRARY})

# hide openAl deprecated on osx warnings
target_compile_options(${LIBRARY_NAME} INTERFACE -W)