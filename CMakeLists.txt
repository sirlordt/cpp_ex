cmake_minimum_required(VERSION 3.15)
project(cpp_integration_test01 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option to enable or disable tests
option(BUILD_TESTS "Build tests" ON)

# Enable testing if BUILD_TESTS is ON
if(BUILD_TESTS)
  enable_testing()
endif()

# Include FetchContent module
include(FetchContent)

# Download and build Catch2 from GitHub
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0 # Using the latest version as of May 2025
)
FetchContent_MakeAvailable(catch2)

# Run the script to modify Catch2's signal handling
execute_process(
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/modify_catch2.sh
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  RESULT_VARIABLE SCRIPT_RESULT
)

if(NOT SCRIPT_RESULT EQUAL 0)
  message(WARNING "Failed to modify Catch2's signal handling. Tests may not work correctly.")
endif()

# Check for try_catch_guard library and add it if available
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/libs/try_catch_guard/src/try_catch_guard.hpp")
  message(STATUS "Found try_catch_guard library, adding it to the project")

  # Create an interface library for try_catch_guard
  add_library(try_catch_guard INTERFACE)
  target_include_directories(try_catch_guard INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/libs/try_catch_guard/src)
  set(TRY_CATCH_GUARD_AVAILABLE TRUE)
else()
  message(WARNING "try_catch_guard library not found at ${CMAKE_CURRENT_SOURCE_DIR}/external/libs/try_catch_guard/src/try_catch_guard.hpp")
  set(TRY_CATCH_GUARD_AVAILABLE FALSE)
endif()

# Load binary name from .dist_build
file(STRINGS ".dist_build" DIST_BUILD_CONTENT)

foreach(LINE ${DIST_BUILD_CONTENT})
  if(LINE MATCHES "^Container_Bin_Name=\"([^\"]+)\"$")
    set(APP_BIN_NAME ${CMAKE_MATCH_1})
  endif()
endforeach()

if(NOT DEFINED APP_BIN_NAME)
  set(APP_BIN_NAME "cpp_integration_test01")
  message(WARNING "Container_Bin_Name not found in .dist_build, using default: ${APP_BIN_NAME}")
endif()

message(STATUS "Building executable: ${APP_BIN_NAME}")

# Find dependencies with Conan
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
  include("${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
endif()

# Enable Address Sanitizer and Undefined Behavior Sanitizer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")

# Add executable
add_executable(${APP_BIN_NAME} main.cpp)

# Link with try_catch_guard if available
if(TRY_CATCH_GUARD_AVAILABLE)
  target_link_libraries(${APP_BIN_NAME} PRIVATE try_catch_guard)
  target_compile_definitions(${APP_BIN_NAME} PRIVATE TRY_CATCH_GUARD_AVAILABLE)

  # Add include directory for the main executable
  target_include_directories(${APP_BIN_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/libs/try_catch_guard/src)
endif()

# Install target
install(TARGETS ${APP_BIN_NAME}
  DESTINATION bin)

# Add tests directory if BUILD_TESTS is ON
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
