cmake_minimum_required(VERSION 3.15)
project(cpp_integration_test01 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing
enable_testing()

# Include FetchContent module
include(FetchContent)

# Download and build Catch2 from GitHub
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0  # Using the latest version as of May 2025
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

# Install target
install(TARGETS ${APP_BIN_NAME}
        DESTINATION bin)

# Add tests directory
add_subdirectory(tests)
