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
    ogg
    GIT_REPOSITORY https://gitlab.xiph.org/xiph/ogg.git
    GIT_TAG main
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(ogg)
target_link_libraries(${LIBRARY_NAME} INTERFACE ogg)

#vorbis
FetchContent_Declare(
    vorbis
    GIT_REPOSITORY https://gitlab.xiph.org/xiph/vorbis.git
    GIT_TAG main
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(vorbis)
target_link_libraries(${LIBRARY_NAME} INTERFACE vorbis)

#openAL
find_package(OpenAL REQUIRED)
target_include_directories(${LIBRARY_NAME} INTERFACE ${OPENAL_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE ${OPENAL_LIBRARY})