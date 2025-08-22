add_library(physics INTERFACE)

target_include_directories(physics INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/collisionDetection/geometry ${CMAKE_CURRENT_SOURCE_DIR}/geometry)

# library link dependencies
target_link_libraries(physics INTERFACE math geometry)
