cmake_minimum_required(VERSION 3.0.0)
project(MoChengEngine VERSION 0.1.0)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD 20)
include_directories(${PROJECT_SOURCE_DIR}/lib/Include ${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR})

add_definitions(-w)
add_subdirectory(FrameWork)
add_subdirectory(G_Example)

execute_process(COMMAND dir .. 
OUTPUT_VARIABLE ls_ret)
message(STATUS "ls output:e4te3 ${ls_ret}")
 

# add_subdirectory(tools)
FIND_LIBRARY(GLFW glfw3 ${PROJECT_SOURCE_DIR}/lib/glfw3)

FIND_LIBRARY(VULKAN vulkan ${PROJECT_SOURCE_DIR}/lib)
add_executable(MoChengEngine main.cpp)
target_link_libraries(MoChengEngine framework_lib example_lib ${VULKAN} ${GLFW})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
