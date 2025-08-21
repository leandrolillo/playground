add_library(audio INTERFACE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(OpenAL REQUIRED)

find_package(Vorbis REQUIRED)
find_package(Ogg REQUIRED)

# make sure this library headers are included
target_include_directories(audio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${OPENAL_INCLUDE_DIR} ${VORBIS_INCLUDE_DIRS} ${OGG_INCLUDE_DIRS})

# library link dependencies
target_link_libraries(audio INTERFACE javaLike log resources math ${OPENAL_LIBRARY} ${VORBIS_LIBRARIES} ${OGG_LIBRARIES})
