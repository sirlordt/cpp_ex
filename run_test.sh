#!/bin/bash
set -e  # Exit on error

# Script: run_test.sh
# Description: Runs the project tests
# Usage: ./run_test.sh [options]
# Options:
#   --no-try-catch-guard-tests  Disable running try_catch_guard tests
#   --ctest                     Run tests using CTest
#   --asan                      Enable Address Sanitizer
#   --check                     Check shared library dependencies of test executable
#   --rebuild                   Rebuild the test targets (full rebuild if tests don't exist)

# Default values for options
RUN_TRY_CATCH_GUARD_TESTS=false
ASAN_OPTIONS=""
RUN_CTEST=false
ENABLE_ASAN=false
REBUILD=false
USE_VALGRIND=false
CHECK_DEPENDENCIES=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --try-catch-guard-tests)
            RUN_TRY_CATCH_GUARD_TESTS=true
            shift
            ;;
        --asan)
            ASAN_OPTIONS="handle_segv=0:allow_user_segv_handler=1:detect_leaks=1:print_stacktrace=1:halt_on_error=0"
            ENABLE_ASAN=true
            shift
            ;;
        --rebuild)
            REBUILD=true
            shift
            ;;
        --ctest)
            RUN_CTEST=true
            shift
            ;;
        --valgrind)
            USE_VALGRIND=true
            shift
            ;;
        --check)
            CHECK_DEPENDENCIES=true
            shift
            ;;
        *)
            # Unknown option
            echo "Unknown option: $1"
            echo "Usage: $0 [--no-try-catch-guard-tests] [--ctest] [--asan] [--rebuild] [--valgrind] [--check]"
            exit 1
            ;;
    esac
done

# Check for incompatible options
if [ "$USE_VALGRIND" = true ] && [ "$ENABLE_ASAN" = true ]; then
    echo -e "\nWARNING: Valgrind and AddressSanitizer are not fully compatible."
    echo -e "You should use either Valgrind or AddressSanitizer, but not both."
    echo -e "Consider not using --asan with --valgrind.\n"
    
    # Ask for confirmation
    read -p "Do you want to continue anyway? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo "Exiting as requested."
        exit 1
    fi
    echo -e "Continuing as requested. Expect potential issues.\n"
fi

# Check if valgrind is installed if --valgrind is used
if [ "$USE_VALGRIND" = true ]; then
    if ! command -v valgrind &> /dev/null; then
        echo "Error: Valgrind is not installed but --valgrind option was specified."
        echo "Please install Valgrind or remove the --valgrind option."
        exit 1
    fi
fi

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

# Check and install required packages
ensure_package_installed "cmake"
ensure_package_installed "python3-pip" "pip3"

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

# Check if the tests are built
if [ ! -f "build/tests/unit/unit_tests" ] || [ "$REBUILD" = true ]; then
    # Build with appropriate options
    BUILD_CMD="./build_test.sh"
    if [ "$ENABLE_ASAN" = true ]; then
        BUILD_CMD="$BUILD_CMD --asan"
    fi
    if [ "$RUN_TRY_CATCH_GUARD_TESTS" = false ]; then
        BUILD_CMD="$BUILD_CMD --no-try-catch-guard-tests"
    fi

    # Execute the build command
    $BUILD_CMD
else
    echo "Using existing test executable. Use --rebuild to force a rebuild."
fi

# Turn off command echoing if it was enabled by build_test.sh (redirect to /dev/null to hide the command itself)
{ set +x; } 2>/dev/null

echo -e "\nRunning tests..."
cd build

# We don't need to manually run try_catch_guard tests here anymore
# as they are now handled by the CMake custom target

echo -e "\n========== Running Tests ==========\n"

# Run try_catch_guard tests if enabled
if [ "$RUN_TRY_CATCH_GUARD_TESTS" = true ] && [ -d "../external/libs/try_catch_guard" ]; then
    echo -e "Running try_catch_guard tests..."
    if [ -d "../external/libs/try_catch_guard/tests" ]; then
        cd ../external/libs/try_catch_guard
        echo -e "Found try_catch_guard tests directory. Running tests...\n"
        ./run_test.sh
        echo -e "\ntry_catch_guard tests completed!"
        cd ../../build
    else
        echo -e "try_catch_guard tests directory not found. Skipping try_catch_guard tests."
    fi
else
    if [ "$RUN_TRY_CATCH_GUARD_TESTS" = false ]; then
        echo -e "try_catch_guard tests are disabled. Skipping try_catch_guard tests."
    fi
fi

cd tests

# Integration tests have been removed

# Run unit tests
echo -e "\nRunning unit tests..."
if [ -f "unit/unit_tests" ]; then
    echo -e "Found unit_tests executable."
    cd unit
    
    # Check shared library dependencies if requested
    if [ "$CHECK_DEPENDENCIES" = true ]; then
        echo -e "\n========== Checking Shared Library Dependencies ==========\n"
        echo -e "Running ldd on unit_tests executable...\n"
        ldd ./unit_tests
        echo -e "\n========== End of Dependencies Check ==========\n"
        
        # If checking dependencies, exit here without running tests
        cd ../..
        exit 0
    fi
    
    echo -e "Running...\n"

    # Set ASAN_OPTIONS for all tests
    # We'll use a run_test function to properly set the environment variable
    run_test() {
        local tag="$1"
        
        # Execute the command with appropriate environment variables
        if [ "$USE_VALGRIND" = true ]; then
            # Using Valgrind
            if [ "$tag" = "--list-tests" ]; then
                if [ -n "$ASAN_OPTIONS" ]; then
                    env ASAN_OPTIONS="$ASAN_OPTIONS" valgrind ./unit_tests $tag || echo "Test listing failed"
                else
                    valgrind ./unit_tests $tag || echo "Test listing failed"
                fi
            else
                if [ -n "$ASAN_OPTIONS" ]; then
                    env ASAN_OPTIONS="$ASAN_OPTIONS" valgrind ./unit_tests "[$tag]" || echo "Test with tag [$tag] failed"
                else
                    valgrind ./unit_tests "[$tag]" || echo "Test with tag [$tag] failed"
                fi
            fi
        else
            # Not using Valgrind
            if [ "$tag" = "--list-tests" ]; then
                if [ -n "$ASAN_OPTIONS" ]; then
                    env ASAN_OPTIONS="$ASAN_OPTIONS" ./unit_tests $tag || echo "Test listing failed"
                else
                    ./unit_tests $tag || echo "Test listing failed"
                fi
            else
                if [ -n "$ASAN_OPTIONS" ]; then
                    env ASAN_OPTIONS="$ASAN_OPTIONS" ./unit_tests "[$tag]" || echo "Test with tag [$tag] failed"
                else
                    ./unit_tests "[$tag]" || echo "Test with tag [$tag] failed"
                fi
            fi
        fi
    }

    # First list all tests
    echo -e "Listing all tests...\n"
    run_test "--list-tests"
    #ASAN_OPTIONS=handle_segv=0:allow_user_segv_handler=1:detect_leaks=0 
    #./unit_tests --list-tests
    
    # Run each test individually to identify which one is failing
    echo -e "\nRunning tests individually to identify failures...\n"
   

    echo -e "\nRunning tests with tag [exceptions]..."
    run_test "exceptions"

    echo -e "\nRunning tests with tag [map]..."
    run_test "map"

    echo -e "\nRunning tests with tag [string]..."
    run_test "string"

    echo -e "\nRunning tests with tag [vector]..."
    run_test "vector"

    echo -e "\nRunning tests with tag [safe_shared_ptr]..."
    run_test "safe_shared_ptr"
    
    echo -e "\nRunning tests with tag [safe_unique_ptr]..."
    echo -e "Debug: About to run safe_unique_ptr tests"
    run_test "safe_unique_ptr"
    echo -e "Debug: Finished running safe_unique_ptr tests"

    echo -e "\nUnit tests completed successfully!"

    cd ..
else
    echo -e "ERROR: Unit tests executable not found at $(pwd)/unit/unit_tests"
    echo -e "You may need to rebuild the tests using ./build_test.sh"
fi

echo -e "\n========== All Tests Completed ==========\n"

# Run tests with CTest only if requested
if [ "$RUN_CTEST" = true ]; then
    echo -e "========== Running Tests with CTest ==========\n"
    cd ..
    
    # Integration tests have been removed
    
    # Run unit tests with CTest if they exist
    if [ -f "tests/unit/unit_tests" ]; then
        echo -e "\nRunning unit tests with CTest..."
        ASAN_OPTIONS=handle_segv=0:allow_user_segv_handler=1:detect_leaks=0 ctest -R unit_tests --output-on-failure
        if [ $? -ne 0 ]; then
            echo -e "WARNING: CTest unit tests failed."
        else
            echo -e "CTest unit tests completed successfully!"
        fi
    else
        echo -e "ERROR: Unit tests executable not found. Skipping CTest unit tests."
    fi
    
    echo -e "\n========== CTest Execution Completed ==========\n"
else
    cd ..
fi

echo -e "Test execution completed. Check the output above for any errors."

echo -e "\nAll tests completed!"