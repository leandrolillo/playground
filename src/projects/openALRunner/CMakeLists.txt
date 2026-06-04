#library
set(LIBRARY_NAME openALRunner)
add_library(${LIBRARY_NAME} INTERFACE)

#sources
add_subdirectory(resources)
add_subdirectory(adapters)
target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE playground audioRunner)

#dependencies
#openAL
find_package(OpenAL REQUIRED)
target_include_directories(${LIBRARY_NAME} INTERFACE ${OPENAL_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} INTERFACE ${OPENAL_LIBRARY})

# hide openAl deprecated on osx warnings
target_compile_options(${LIBRARY_NAME} INTERFACE -W)
