cmake_minimum_required(VERSION 3.15)
project(cpp_ex VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_TRY_CATCH_GUARD_TESTS "Build and run try_catch_guard tests" ON)
option(ENABLE_ASAN "Enable Address Sanitizer" OFF)

# Enable testing if BUILD_TESTS is ON
if(BUILD_TESTS)
  enable_testing()
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
  set(APP_BIN_NAME "cpp_ex")
  message(WARNING "Container_Bin_Name not found in .dist_build, using default: ${APP_BIN_NAME}")
endif()

message(STATUS "Building executable: ${APP_BIN_NAME}")

# Find dependencies with Conan
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
  include("${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
endif()

# Enable Address Sanitizer and Undefined Behavior Sanitizer if ENABLE_ASAN is ON
if(ENABLE_ASAN)
  message(STATUS "Address Sanitizer enabled")

  # Ensure ASAN runtime is linked first
  set(CMAKE_CXX_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer ${CMAKE_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address,undefined ${CMAKE_EXE_LINKER_FLAGS}")

  # Add compile definition to indicate ASAN is enabled
  add_compile_definitions(ASAN_ENABLED)
else()
  message(STATUS "Address Sanitizer disabled")
endif()

# Add core library headers (header-only)
add_library(cpp_ex_core INTERFACE)
target_include_directories(cpp_ex_core INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/libs)

# Add executable
add_executable(${APP_BIN_NAME} src/main.cpp)

# Link with core library (header-only)
target_link_libraries(${APP_BIN_NAME} INTERFACE cpp_ex_core)

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
