#!/bin/bash

# build.sh
# Script to automate the build process for the C++ Extended Library
# Usage: ./build.sh [-test] [--no-asan] (use -test to build and run tests, --no-asan to disable Address Sanitizer)

set -e

# Parse command line arguments
BUILD_TESTS=0
ENABLE_ASAN=1

for arg in "$@"; do
    case $arg in
        -test)
            BUILD_TESTS=1
            ;;
        --no-asan)
            ENABLE_ASAN=0
            ;;
    esac
done

if [ $BUILD_TESTS -eq 1 ]; then
    echo "Building C++ Extended Library with tests..."
else
    echo "Building C++ Extended Library without tests..."
fi

if [ $ENABLE_ASAN -eq 1 ]; then
    echo "Address Sanitizer enabled"
else
    echo "Address Sanitizer disabled"
fi

# Function to check if a directory is a valid CMake project
is_valid_cmake_project() {
    local dir="$1"
    if [ -f "${dir}/CMakeLists.txt" ]; then
        return 0  # True
    else
        return 1  # False
    fi
}

# Function to download try_catch_guard library
download_try_catch_guard() {
    echo "Downloading try_catch_guard library from GitHub..."
    
    # Create directories if they don't exist
    mkdir -p external/libs
    
    # Clone the repository
    if git clone https://github.com/sirlordt/try_catch_guard external/libs/try_catch_guard; then
        echo "Successfully downloaded try_catch_guard library."
        return 0
    else
        echo "ERROR: Failed to download try_catch_guard library from GitHub."
        return 1
    fi
}

# Check if try_catch_guard library exists and is a valid CMake project
if [ -d "external/libs/try_catch_guard" ] && is_valid_cmake_project "external/libs/try_catch_guard"; then
    echo "try_catch_guard library found and appears to be a valid CMake project."
else
    echo "try_catch_guard library not found or not a valid CMake project."
    
    # Download try_catch_guard library
    if ! download_try_catch_guard; then
        echo "Aborting build due to failure to download try_catch_guard library."
        exit 1
    fi
fi

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Generate Conan files
echo "Generating Conan files..."
conan install .. --output-folder=. --build=missing

# Configure with CMake
echo "Configuring with CMake..."

# Build the CMake command with options
CMAKE_OPTS="-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug"

# Add test option
if [ $BUILD_TESTS -eq 1 ]; then
    CMAKE_OPTS="$CMAKE_OPTS -DBUILD_TESTS=ON"
else
    CMAKE_OPTS="$CMAKE_OPTS -DBUILD_TESTS=OFF"
fi

# Add ASAN option
if [ $ENABLE_ASAN -eq 1 ]; then
    CMAKE_OPTS="$CMAKE_OPTS -DENABLE_ASAN=ON"
else
    CMAKE_OPTS="$CMAKE_OPTS -DENABLE_ASAN=OFF"
fi

# Run CMake with all options
cmake .. $CMAKE_OPTS

# Build the project
echo "Building the project..."
cmake --build .

echo "Build completed successfully!"

# Get the binary name from .dist_build using a more precise method
if [ -f "../.dist_build" ]; then
    BIN_NAME=$(awk -F'"' '/^Container_Bin_Name=/{print $2}' ../.dist_build)
else
    BIN_NAME="cpp_ex"
fi


if [ $BUILD_TESTS -eq 1 ]; then
    echo "You can run the application with: ./build/${BIN_NAME}"
    echo "You can run the tests with: cd build && ctest"
else
    echo "You can run the application with: ./build/${BIN_NAME}"
fi
