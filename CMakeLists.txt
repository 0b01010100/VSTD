cmake_minimum_required(VERSION 3.22)

project(VUtils LANGUAGES C CXX)

# Set output directories for different target types
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_DEBUG_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(VUtils STATIC Utils/src/v.c "Utils/src/cstring.c")

target_include_directories(VUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(VUtils_Test test/main.c "Utils/src/cstring.c")

target_link_libraries(VUtils_Test PUBLIC VUtils)