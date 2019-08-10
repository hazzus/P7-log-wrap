cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(incoel_logger)

include_directories(${PROJECT_SOURCE_DIR}/logger)

add_executable(${PROJECT_NAME} main.cpp logger/logger.h logger/logger.cpp)

add_subdirectory(${PROJECT_SOURCE_DIR}/libp7-baical)

target_link_libraries(${PROJECT_NAME} P7)
