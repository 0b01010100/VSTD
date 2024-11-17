cmake_minimum_required(VERSION 3.20)
add_compile_definitions(-DUNICODE)
project(vstd LANGUAGES C CXX)

# Export our VG_ functions to a .dll file
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Collect all source files
file(GLOB_RECURSE vstd_SOURCES CONFIGURE_DEPENDS 
  vstd/*.c
  vstd/containers/*.c
  vstd/debug/*.c
  vstd/identifiers/*.c
  vstd/math/*.c
  vstd/memory/*.c
  vstd/platform/*.c
  vstd/strings/*.c
  vstd/threads/*.c
  vstd/time/*.c
)

# Define the vstd library
add_library(vstd SHARED ${vstd_SOURCES})

# Set include directories properly
target_include_directories(vstd 
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vstd
)

# Create the executable and link it to the vstd library
add_executable(test main.c)
target_link_libraries(test PUBLIC vstd)

# Set output directories for the test executable
set_target_properties(test 
  PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  DEBUG_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

add_custom_command(TARGET test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CMAKE_BINARY_DIR}/vstd.dll"
  $<TARGET_FILE_DIR:test>)