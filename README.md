# C++ Development Environment with Distrobox

This project provides scripts to set up a C++ development environment using Distrobox and create a containerized C++ application.

## Features

- **Complete C++ Development Environment**: This project is set up with everything you need for modern C++ development:
  - **C++23 Support**: Latest C++ standard features are enabled
  - **Modern Build System**: CMake 3.28.3 with proper project structure
  - **Package Management**: Conan integration for dependency management
  - **IDE Integration**: Full VSCode setup with debugging capabilities
  - **Containerization**: Tools to package your application in a minimal Docker container
  - **Docker Support**: Docker Engine installed from the official Docker repository
  - **Testing Framework**: Catch2 v3.4.0 for unit testing
  - **Code Quality**: Address and Undefined Behavior Sanitizers enabled

- **Development Tools Available**:
  - **Build Tools**: build-essential, CMake 3.28.3
  - **Package Manager**: Conan for C++ dependencies
  - **Version Control**: git for source control
  - **Utilities**: mc (Midnight Commander), htop for system monitoring
  - **IDE**: Visual Studio Code with C++ extensions
  - **Containerization**: Docker Engine and Docker Compose for building and running containers
  - **Testing**: Catch2 for unit testing

- **Project Structure**:
  - Modern CMake project layout
  - Proper separation of build configuration
  - Environment variable management
  - Containerization support
  - Unit tests in the `tests` directory

## Scripts

### build.sh

This script automates the build process for the C++ application:

1. Creates the build directory if it doesn't exist
2. Generates Conan files
3. Configures the project with CMake
4. Builds the project

### build.dist.sh

This script generates a Dockerfile for the C++ application and builds the Docker image:

1. Processes variables from `.dist_build` and `.env_dist` files
2. Builds the project to detect dependencies
3. Analyzes dependencies to determine required packages
4. Generates a Dockerfile with only the necessary dependencies
5. Builds and tags the Docker image

### build_test.sh

This script builds the unit tests:

1. Creates the build directory if it doesn't exist
2. Generates Conan files
3. Configures the project with CMake
4. Builds only the unit tests

### run_test.sh

This script runs the unit tests:

1. Builds the tests if they haven't been built yet
2. Runs the tests and displays the results

### run.sh

This script runs the main application:

1. Builds the application if it hasn't been built yet
2. Runs the application

### clean_build.sh

This script cleans the build directory:

1. Removes the build directory
2. Creates a new empty build directory

### modify_catch2.sh

This script modifies Catch2's signal handling to avoid conflicts:

1. Finds the Catch2 signal handling implementation
2. Disables the signal handling code
3. Replaces it with empty implementations

## Testing

The project includes a comprehensive testing framework using Catch2. The tests are located in the `tests` directory and can be built and run using the provided scripts.

### Running Tests

To run the tests, use the `run_test.sh` script:

```bash
./run_test.sh
```

### Writing Tests

Tests are written using the Catch2 framework. Here's a simple example:

```cpp
#include <catch2/catch_test_macros.hpp>

TEST_CASE("My test case", "[tag]") {
    REQUIRE(1 + 1 == 2);
}
```

## Code Quality

The project is configured with Address Sanitizer and Undefined Behavior Sanitizer to catch memory errors and undefined behavior at runtime. These sanitizers are enabled for both the main application and the tests.

The sanitizers help catch issues such as:
- Buffer overflows
- Use-after-free
- Memory leaks
- Null pointer dereferences
- Integer overflows
- Division by zero
- And many other runtime errors

To run the application with sanitizers enabled, simply use the `run.sh` script:

```bash
./run.sh
