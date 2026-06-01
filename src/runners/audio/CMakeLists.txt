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
find_package(Ogg REQUIRED)
target_include_directories(${LIBRARY_NAME} INTERFACE ${OGG_INCLUDE_DIRS} ${OGG_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE ${OGG_LIBRARIES} ${OGG_LIBRARY})

#vorbis
find_package(Vorbis REQUIRED)
target_include_directories(${LIBRARY_NAME} INTERFACE ${VORBIS_INCLUDE_DIRS})
target_link_libraries(${LIBRARY_NAME} INTERFACE ${VORBIS_LIBRARIES})

#openAL
find_package(OpenAL REQUIRED)
target_include_directories(${LIBRARY_NAME} INTERFACE ${OPENAL_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE ${OPENAL_LIBRARY})

# hide openAl deprecated on osx warnings
target_compile_options(${LIBRARY_NAME} INTERFACE -W)