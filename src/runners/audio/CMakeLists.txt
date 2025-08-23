set(LIBRARY_NAME audioRunner)
add_library(${LIBRARY_NAME} INTERFACE)

add_subdirectory(adapters)
add_subdirectory(resources)

target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE playground )

#dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

#OpenAL
find_package(OpenAL REQUIRED)
target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${OPENAL_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE ${OPENAL_LIBRARY})

#Ogg
find_package(Ogg REQUIRED)
target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${OGG_INCLUDE_DIRS})
target_link_libraries(${LIBRARY_NAME} INTERFACE ${OGG_LIBRARIES})

#Vorbis
find_package(Vorbis REQUIRED)
target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${VORBIS_INCLUDE_DIRS})
target_link_libraries(${LIBRARY_NAME} INTERFACE ${VORBIS_LIBRARIES})