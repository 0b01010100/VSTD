cmake_minimum_required(VERSION 3.22)

project(VUTLlS_TEST LANGUAGES C CXX)


#check Platform
if(WIN32 OR _WIN32 OR WIN64 OR _WIN64)
    # Windows-specific setup
    message(STATUS "Configuring for Windows")
    add_definitions(-DU_WINDOWS)
    # Add Windows-specific definitions, include directories, or other configurations
elseif(APPLE)
    # macOS-specific setup
    message(STATUS "Configuring for macOS")
    add_definitions(-DU_APPLE)
    # Add macOS-specific definitions, include directories, or other configurations
elseif(UNIX)
    # Unix-like platform (Linux, BSD, etc.) setup
    message(STATUS "Configuring for Unix-like platform")
    add_definitions(-DU_UNIX)
    # Add Unix-like platform-specific definitions, include directories, or other configurations
else()
    message(WARNING "Unsupported platform")
    add_definitions(-DU_UNKNOWN)
endif()

add_subdirectory(Utils)

add_executable(VUTLlS_TEST test/console.c)

target_link_libraries(VUTLlS_TEST PUBLIC VUtils)

set_target_properties(VUTLlS_TEST PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  DEBUG_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)