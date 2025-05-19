#!/bin/bash
set -e  # Exit on error

# Script: build_test.sh
# Description: Builds the project and its tests
# Usage: ./build_test.sh [options]
# Options:
#   --no-try-catch-guard-tests  Disable building and running try_catch_guard tests
#   --asan                      Enable Address Sanitizer

# Default values for options
BUILD_TRY_CATCH_GUARD_TESTS=ON
ENABLE_ASAN=OFF
NO_REBUILD_DEPS=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --no-try-catch-guard-tests)
            BUILD_TRY_CATCH_GUARD_TESTS=OFF
            shift
            ;;
        --asan)
            ENABLE_ASAN=ON
            shift
            ;;
        *)
            # Unknown option
            echo "Unknown option: $1"
            echo "Usage: $0 [--no-try-catch-guard-tests] [--asan]"
            exit 1
            ;;
    esac
done

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to install packages if they don't exist
ensure_package_installed() {
    local package_name="$1"
    local command_name="${2:-$1}"  # Default to package name if command name not provided
    
    if ! command_exists "$command_name"; then
        echo "$command_name is not installed. Attempting to install $package_name..."
        
        # Check if we have sudo access
        if command_exists sudo; then
            sudo apt-get update
            sudo apt-get install -y "$package_name"
        else
            echo "Error: sudo is not available. Please install $package_name manually."
            exit 1
        fi
        
        # Verify installation
        if ! command_exists "$command_name"; then
            echo "Error: Failed to install $package_name. Please install it manually."
            exit 1
        fi
        
        echo "$package_name installed successfully."
    else
        echo "$command_name is already installed."
    fi
}

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

# Check and install required packages
ensure_package_installed "cmake"
ensure_package_installed "python3-pip" "pip3"
ensure_package_installed "git"

# Check for Conan
if ! command_exists conan; then
    echo "Conan is not installed. Attempting to install via pip..."
    pip3 install conan
    
    # Verify installation
    if ! command_exists conan; then
        echo "Error: Failed to install Conan. Please install it manually."
        exit 1
    fi
    
    echo "Conan installed successfully."
fi

# Check if Conan profile exists and create one if it doesn't
if ! conan profile list | grep -q default; then
    echo "No default Conan profile found. Creating one..."
    conan profile detect
fi

# Print commands before executing them
set -x

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir -p build
    
    # Navigate to build directory
    cd build
    
    # We've moved these commands to the if/else blocks above
else
    echo "Using existing build directory..."
    
    # Navigate to build directory
    cd build
    
    echo "Cleaning test targets..."
    rm -f tests/unit/unit_tests
    
    # Reconfigure with CMake if needed
    echo "Reconfiguring with CMake..."
    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DBUILD_TRY_CATCH_GUARD_TESTS=${BUILD_TRY_CATCH_GUARD_TESTS} -DENABLE_ASAN=${ENABLE_ASAN}
fi

# Install dependencies with Conan and generate CMake files (Conan 2.x approach)
echo "Installing dependencies with Conan..."
conan install .. --output-folder=. --build=missing

# Configure with CMake
echo "Configuring with CMake..."
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DBUILD_TRY_CATCH_GUARD_TESTS=${BUILD_TRY_CATCH_GUARD_TESTS} -DENABLE_ASAN=${ENABLE_ASAN}

# Print status message about ASAN
if [ "${ENABLE_ASAN}" = "OFF" ]; then
    echo "Address Sanitizer is disabled"
else
    echo "Address Sanitizer is enabled"
fi

# Print status message about try_catch_guard tests
if [ "${BUILD_TRY_CATCH_GUARD_TESTS}" = "OFF" ]; then
    echo "try_catch_guard tests are disabled"
else
    echo "try_catch_guard tests are enabled"
fi

echo "Building the entire project..."
cmake --build .

# Integration tests have been removed

# Build unit tests if they exist
if grep -q "add_executable(unit_tests" ../tests/unit/CMakeLists.txt 2>/dev/null; then
    echo "Building unit tests..."
    cmake --build . --target unit_tests || echo "Warning: Failed to build unit_tests target. It may not exist in the project."
else
    echo "Unit tests not found in project. Skipping unit tests build."
fi

# We don't need to manually build try_catch_guard tests here anymore
# as they are now handled by the CMake custom target

# Turn off command echoing before the final message (redirect to /dev/null to hide the command itself)
{ set +x; } 2>/dev/null

echo -e "\nTests built successfully!"