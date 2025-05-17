#!/bin/bash
set -e  # Exit on error

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

# Check if build directory exists, create if it doesn't
if [ ! -d "build" ]; then
    mkdir -p build
fi

# Navigate to build directory
cd build

# Install dependencies with Conan and generate CMake files (Conan 2.x approach)
conan install .. --output-folder=. --build=missing

# Configure with CMake
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

# Build the tests
cmake --build . --target integration_tests

# We don't need to manually build try_catch_guard tests here anymore
# as they are now handled by the CMake custom target

# Turn off command echoing before the final message (redirect to /dev/null to hide the command itself)
{ set +x; } 2>/dev/null

echo -e "\nTests built successfully!"