#Library
set(LIBRARY_NAME audioRunner)
add_library(${LIBRARY_NAME} STATIC AudioRunner.cpp)

#sources
add_subdirectory(resources)
add_subdirectory(adapters)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} PUBLIC playground)

#dependencies
#ogg
FetchContent_Declare(
    Ogg
    GIT_REPOSITORY https://github.com/xiph/ogg.git
    GIT_TAG ${OGG_VERSION}
    GIT_SHALLOW TRUE
    FIND_PACKAGE_ARGS NAMES Ogg)
FetchContent_MakeAvailable(Ogg)
target_link_libraries(${LIBRARY_NAME} PUBLIC Ogg::ogg)

#vorbis
FetchContent_Declare(
    Vorbis
    GIT_REPOSITORY https://github.com/xiph/vorbis.git
    GIT_TAG ${VORBIS_VERSION}
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(Vorbis)
target_link_libraries(${LIBRARY_NAME} PUBLIC vorbis vorbisfile)