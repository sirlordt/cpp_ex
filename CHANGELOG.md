# Changelog

## 2025-05-18 09:48 (America/Los_Angeles, UTC-7:00)

### Core Library Integration

#### Project Structure
* Added core library headers in src/libs/core/
* Created INTERFACE library in CMakeLists.txt for header-only libraries
* Fixed namespace inconsistencies in header files
* Updated main.cpp to use the core library classes and functions
* Added examples for SafeSharedPtr and SafeUniquePtr usage

#### CMakeLists.txt
* Added cpp_ex_core INTERFACE library for header-only libraries
* Configured include directories to make headers available project-wide
* Linked the main executable with the core library

#### SafeUniquePtr Implementation
* Added array specialization for SafeUniquePtr class
* Implemented array access operator with null pointer checks
* Fixed makeSafeUnique function for array types
* Added comprehensive examples demonstrating usage

## 2025-05-18 09:12 (America/Los_Angeles, UTC-7:00)

### Code Structure Reorganization

#### Project Structure
* Moved main.cpp from the root directory to src/ directory
* Overwrote the existing src/main.cpp file with the more complete version
* Removed the main.cpp file from the project root
* Updated CMakeLists.txt to use src/main.cpp instead of main.cpp

#### CMakeLists.txt
* Changed add_executable line to use src/main.cpp instead of main.cpp
* Verified that everything compiles and runs correctly

### Enhanced Container Dependency Detection

#### build.dist.sh
* Implemented automatic detection of shared libraries (.so) using ldd
* Added mapping of known libraries to their corresponding packages
* Created function to add packages and track their versions
* Added logic to determine which Debian packages provide the detected libraries
* Ensured that the Dockerfile includes all necessary packages
* Added special handling for libraries like libasan.so.6, libstdc++.so.6, and libubsan.so.1
* Verified that the Docker container builds and runs correctly with all dependencies

## 2025-05-16 20:35 (America/Los_Angeles, UTC-7:00)

### Added Optional Test Building

#### CMakeLists.txt
* Added BUILD_TESTS option to control test building
* Modified enable_testing() to only run when BUILD_TESTS is ON
* Conditioned tests directory inclusion on BUILD_TESTS option

#### build.sh
* Added parameter support for -test flag
* Modified script to accept optional -test parameter
* Added conditional CMake configuration based on test flag
* Updated output messages to indicate build mode (with/without tests)
* Simplified build process by using CMake options instead of file manipulation

## 2025-05-16 20:23 (America/Los_Angeles, UTC-7:00)

### Added try_catch_guard Test in main.cpp

#### main.cpp
* Added _try _catch blocks to test try_catch_guard functionality
* Created test for null pointer exception handling
* Created test for standard exception handling
* Verified successful integration with try_catch_guard library

#### CMakeLists.txt
* Added include directory for try_catch_guard to the main executable
* Simplified include path in main.cpp

## 2025-05-16 20:20 (America/Los_Angeles, UTC-7:00)

### Fixed try_catch_guard Integration

#### CMakeLists.txt
* Fixed integration with try_catch_guard as a header-only library
* Created an INTERFACE library target for try_catch_guard
* Updated include directories to point to the correct location

#### tests/CMakeLists.txt
* Updated to use the try_catch_guard interface library
* Added custom target to build and run try_catch_guard tests separately
* Improved test integration with the main project
* Fixed CTest integration by using direct test execution instead of test discovery
* Removed automatic test discovery which was causing segfaults with signal handling

#### run_test.sh and build_test.sh
* Removed manual handling of try_catch_guard tests
* Tests are now handled by the CMake custom target
* Updated CTest execution to only run direct tests to avoid segfaults

## 2025-05-16 20:06 (America/Los_Angeles, UTC-7:00)

### Library Name Change: memory_guard → try_catch_guard

#### build.sh
* Updated repository URL from https://github.com/sirlordt/memory_guard to https://github.com/sirlordt/try_catch_guard
* Renamed all function names and variables from memory_guard to try_catch_guard
* Updated directory paths from external/libs/memory_guard to external/libs/try_catch_guard

#### CMakeLists.txt
* Changed library check path from external/libs/memory_guard to external/libs/try_catch_guard
* Renamed variable MEMORY_GUARD_AVAILABLE to TRY_CATCH_GUARD_AVAILABLE
* Updated linking and compile definitions to use try_catch_guard

#### tests/CMakeLists.txt
* Updated library linking from memory_guard to try_catch_guard
* Changed compile definition from MEMORY_GUARD_AVAILABLE to TRY_CATCH_GUARD_AVAILABLE
* Updated test target references from memory_guard_tests to try_catch_guard_tests

#### run_test.sh
* Updated test execution path from external/libs/memory_guard/memory_guard_tests to external/libs/try_catch_guard/try_catch_guard_tests
* Changed log messages to reference try_catch_guard instead of memory_guard

#### build_test.sh
* Updated repository URL and download function name
* Changed directory checks and test target references
* Updated build target from memory_guard_tests to try_catch_guard_tests

#### Documentation
* Updated all memory bank files (activeContext.md, systemPatterns.md, progress.md, techContext.md)
* Changed library purpose description from "memory management" to "exception handling"
* Updated all references to the library name and repository URL