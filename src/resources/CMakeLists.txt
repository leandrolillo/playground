add_library(resources ResourceManager.cpp FileParser.cpp ResourceLoadResponse.cpp)
target_include_directories(resources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_SOURCE_DIR}/adapters)

# library link dependencies
target_link_libraries(resources PUBLIC javaLike log exceptions)
