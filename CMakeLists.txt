cmake_minimum_required(VERSION 3.20)

project(VUTLlS LANGUAGES C CXX)

# Collect all source files
file(GLOB_RECURSE vstd_SOURCES CONFIGURE_DEPENDS 
  vstd/console/*.c
  vstd/containers/*.c
  vstd/dlls/*.c
  vstd/files/*.c
  vstd/threads/*.c)

# Create the static library
add_library(vstd STATIC ${vstd_SOURCES})

# Set include directories properly
target_include_directories(vstd 
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vstd
    ${CMAKE_CURRENT_SOURCE_DIR}/vstd/console
    ${CMAKE_CURRENT_SOURCE_DIR}/vstd/containers
    ${CMAKE_CURRENT_SOURCE_DIR}/vstd/defines
    ${CMAKE_CURRENT_SOURCE_DIR}/vstd/dlls
    ${CMAKE_CURRENT_SOURCE_DIR}/vstd/files
    ${CMAKE_CURRENT_SOURCE_DIR}/vstd/threads
)

macro(test CASE_VAR)
  set(_switch_var ${CASE_VAR})

  # Check each case
  if(${_switch_var} STREQUAL "array")
    set(TEST_NAME "array_test")
    set(TEST_SOURCE "test/varray.c")
  elseif(${_switch_var} STREQUAL "console")
    set(TEST_NAME "console_test")
    set(TEST_SOURCE "test/vconsole.c")
  elseif(${_switch_var} STREQUAL "logger")
    set(TEST_NAME "logger_test")
    set(TEST_SOURCE "test/vlogger.c")
  elseif(${_switch_var} STREQUAL "other")
    set(TEST_NAME "other_test")
    set(TEST_SOURCE "test/other.c")
  elseif(${_switch_var} STREQUAL "stack")
    set(TEST_NAME "stack_test")
    set(TEST_SOURCE "test/vsk.c")
  elseif(${_switch_var} STREQUAL "thr")
    set(TEST_NAME "thr_test")
    set(TEST_SOURCE "test/vthr.c")
  elseif(${_switch_var} STREQUAL "vec")
    set(TEST_NAME "vec_test")
    set(TEST_SOURCE "test/vvec.c")
  elseif(${_switch_var} STREQUAL "vll")
    set(TEST_NAME "vll_test")
    set(TEST_SOURCE "test/vll.c")
  else()
    set(TEST_NAME "default_test")
    set(TEST_SOURCE "test/default.c")
  endif()

  message("Running ${TEST_NAME} with source ${TEST_SOURCE}")

  # Create the test executable
  add_executable(${TEST_NAME} ${TEST_SOURCE})
  target_include_directories(${TEST_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vstd
  )

  # Link the test executable to the vstd library
  target_link_libraries(${TEST_NAME} PUBLIC vstd)
endmacro()

# Example usage
test("vll")

# Set output directories
set_target_properties(vstd 
  PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  DEBUG_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)