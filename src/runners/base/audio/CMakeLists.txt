#Library
set(LIBRARY_NAME audioRunner)
add_library(${LIBRARY_NAME} STATIC AudioRunner.cpp AudioSource.cpp)

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
    FIND_PACKAGE_ARGS NAMES Ogg
    SOURCE_DIR "${THIRD_PARTY_DEPS_DIR}/ogg-src"
    BINARY_DIR    "${THIRD_PARTY_DEPS_DIR}/ogg-build"
    SUBBUILD_DIR  "${THIRD_PARTY_DEPS_DIR}/ogg-subbuild"
    )
FetchContent_MakeAvailable(Ogg)
target_link_libraries(${LIBRARY_NAME} PUBLIC Ogg::ogg)

#vorbis
FetchContent_Declare(
    Vorbis
    GIT_REPOSITORY https://github.com/xiph/vorbis.git
    GIT_TAG ${VORBIS_VERSION}
    GIT_SHALLOW TRUE
    SOURCE_DIR "${THIRD_PARTY_DEPS_DIR}/vorbis-src"
    BINARY_DIR    "${THIRD_PARTY_DEPS_DIR}/vorbis-build"
    SUBBUILD_DIR  "${THIRD_PARTY_DEPS_DIR}/vorbis-subbuild"
    )
FetchContent_MakeAvailable(Vorbis)
target_link_libraries(${LIBRARY_NAME} PUBLIC vorbis vorbisfile)