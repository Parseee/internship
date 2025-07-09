cmake_minimum_required(VERSION 3.10)
project(graphs CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(project_compiler_options INTERFACE)
target_compile_options(project_compiler_options INTERFACE
    $<$<CONFIG:Debug>:-Wall -Wextra -g -fno-omit-frame-pointer -fsanitize=address,leak>
    $<$<CONFIG:Release>:-O2>
)

find_package(Python REQUIRED)

add_custom_target(
    check
    COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/checker.py $<TARGET_FILE:graphs>
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS graphs
    COMMENT "Test execution and checks"
)


include_directories(src include)

set(SOURCES 
    src/main.cpp
    include/graph.h
    src/graph.cpp
    src/graph_dump.cpp
    src/graph_algo.cpp
    include/node.h
    src/node.cpp
    include/edge.h
    src/edge.cpp)

    
add_executable(${PROJECT_NAME} ${SOURCES})
# add target_link_options
target_link_libraries(${PROJECT_NAME} PRIVATE project_compiler_options 
    $<$<CONFIG:Debug>:-fsanitize=address,leak> 
    $<$<CONFIG:Release>:-O2>)