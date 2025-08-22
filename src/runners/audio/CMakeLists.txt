set(LIBRARY_NAME audio_runner)
add_library(${LIBRARY_NAME} INTERFACE)

#dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
find_package(OpenAL REQUIRED)
find_package(Vorbis REQUIRED)
find_package(Ogg REQUIRED)

add_subdirectory(adapters)
add_subdirectory(resources)

# make sure this library headers are included
target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${OPENAL_INCLUDE_DIR} ${VORBIS_INCLUDE_DIRS} ${OGG_INCLUDE_DIRS})

# library link dependencies
target_link_libraries(${LIBRARY_NAME} INTERFACE playground ${OPENAL_LIBRARY} ${VORBIS_LIBRARIES} ${OGG_LIBRARIES})
