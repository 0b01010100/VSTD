cmake_minimum_required(VERSION 3.22)

project(VUTLlS LANGUAGES C CXX)

# Set output directories for different target types


file(GLOB_RECURSE vstd_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(vstd STATIC ${vstd_SOURCES} )

target_include_directories(vstd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(vstd PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  DEBUG_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)