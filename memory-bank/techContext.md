# Technical Context: C++ Extended Library

## Technologies Used

### Core Technologies

| Technology | Version | Purpose |
|------------|---------|---------|
| C++ | C++23 | Primary programming language |
| CMake | 3.28.3 | Build system |
| Conan | Latest | Package manager |
| Distrobox | Latest | Development environment containerization |
| Docker | Latest | Application containerization |
| Ubuntu | 22.04 | Base OS for containers |

### Dependencies

| Dependency | Version | Purpose |
|------------|---------|---------|
| fmt | 9.1.0 | Modern formatting library for C++ |
| Catch2 | 3.4.0 | Modern C++ testing framework |
| try_catch_guard | Latest | Exception handling library |
| cpp_ex_core | N/A | Internal header-only library for safe pointers |

### Development Tools

| Tool | Purpose |
|------|---------|
| Visual Studio Code | Primary IDE |
| GCC | C++ compiler |
| Git | Version control and library download |
| Midnight Commander (mc) | File management |
| htop | System monitoring |
| nano | Text editing |
| SQLite3 | Database for AI assistant configuration |
| jq | JSON processing for AI assistant configuration |

## Development Setup

### Environment Setup

The development environment is set up using Distrobox, which provides a containerized environment with all necessary tools pre-installed. This ensures consistency across different development machines.

#### Key Components:
- **build-essential**: Provides essential build tools
- **CMake**: Modern build system
- **Conan**: C++ package manager
- **Git**: Version control
- **VSCode**: IDE with C++ extensions
- **nano**: Simple text editor for quick edits

#### VSCode Configuration:
- **Extensions**: C/C++, CMake Tools, Docker, Claude AI
- **Debugging**: Configured for C++ applications
- **Password Store**: Set to "basic" to prevent credential prompts
  ```json
  {
    "password-store": "basic"
  }
  ```
- **Git Configuration**: Disabled Git features in VSCode
  ```json
  {
    "git.enabled": false,
    "git.useCredentialStore": false,
    "git.autofetch": false,
    "git.confirmSync": false,
    "extensions.ignoreRecommendations": false
  }
  ```
- **Auto-Install Extensions**: Task configured to run on folder open
  ```json
  {
    "label": "Auto install extensions",
    "type": "shell",
    "command": "jq -r '.recommendations[]' .vscode/extensions.json | xargs -L 1 code --install-extension",
    "runOptions": {
      "runOn": "folderOpen"
    },
    "presentation": {
      "reveal": "silent"
    }
  }
  ```

#### Shell Configuration:
- **Custom Prompt**: Configured for better visibility in Distrobox container
  ```bash
  # Custom prompt for Distrobox container
  # Make CONTAINER_NAME available as an environment variable
  if [ -z "$CONTAINER_NAME" ]; then
      export CONTAINER_NAME="$HOSTNAME"
  fi
  export PS1="(\h) \u@\[\e[1;32m\]$CONTAINER_NAME\[\e[0m\]:\w\$ "
  ```
- **Login Shell Setup**: Ensures .bashrc is sourced from .bash_profile
  ```bash
  [[ -f ~/.bashrc ]] && source ~/.bashrc
  ```
- **Environment Variables**: CONTAINER_NAME added to system-wide environment variables
  ```bash
  echo "CONTAINER_NAME=$CONTAINER_NAME" | sudo tee -a /etc/environment
  ```

### Build Configuration

The build configuration is managed through CMake with the following key settings:

```cmake
cmake_minimum_required(VERSION 3.15)
project(cpp_ex VERSION 0.1.0 LANGUAGES CXX)

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
  GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

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

# Add core library headers
add_library(cpp_ex_core INTERFACE)
target_include_directories(cpp_ex_core INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/libs)

# Enable Address Sanitizer and Undefined Behavior Sanitizer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
```

### Dependency Management

Dependencies are managed through Conan, with the following configuration in `conanfile.txt`:

```
[requires]
fmt/9.1.0

[generators]
CMakeDeps
CMakeToolchain
```

### Build Process

The build process is automated through the `build.sh` script, which:
1. Checks for command-line parameters (-test flag for building with tests)
2. Checks for try_catch_guard library, downloads if needed
3. Creates the build directory
4. Generates Conan files
5. Configures the project with CMake (with or without tests based on parameters)
6. Builds the project

The script can be run in two modes:
- `./build.sh`: Builds only the main executable without tests
- `./build.sh -test`: Builds the main executable and tests

### Test Build Process

The test build process is automated through the `build_test.sh` script, which:
1. Checks for try_catch_guard library, downloads if needed
2. Checks for and installs required dependencies
3. Creates the build directory
4. Generates Conan files
5. Configures the project with CMake
6. Builds the test targets (integration_tests and try_catch_guard_tests)

### Test Execution Process

The test execution process is automated through the `run_test.sh` script, which:
1. Checks for and installs required dependencies
2. Builds tests if they don't exist
3. Executes the try_catch_guard tests if available
4. Executes the integration tests
5. Runs all tests with CTest
6. Reports test results

### External Library Integration

The external library integration process is managed through the `build.sh` script, which:
1. Checks if the try_catch_guard library exists and is a valid CMake project
2. If not, creates the necessary directories
3. Downloads the try_catch_guard library from GitHub
4. Aborts the build if the download fails
5. Makes the library available to the project through CMake

### Containerization

The containerization process is managed through the `build.dist.sh` script, which:
1. Processes variables from configuration files
2. Builds the project to detect dependencies
3. Generates a Dockerfile with minimal dependencies
4. Builds and tags the Docker image

## Technical Constraints

### Language Constraints
- Must adhere to C++23 standard
- Must compile with GCC compiler

### Build System Constraints
- Must use CMake 3.15 or higher
- Must support Conan integration

### Dependency Constraints
- External dependencies must be managed through Conan
- Dependencies should be pinned to specific versions
- External libraries from GitHub must be properly integrated with CMake

### Testing Constraints
- Must use Catch2 as the testing framework
- Must support Address Sanitizer and Undefined Behavior Sanitizer
- Must modify Catch2's signal handling to work with sanitizers
- Must integrate and run try_catch_guard tests alongside project tests

### Containerization Constraints
- Docker containers must be based on Ubuntu 22.04
- Containers must include only necessary dependencies
- Container images must be properly tagged

## Tool Usage Patterns

### CMake Usage

```cmake
# Load binary name from .dist_build
file(STRINGS ".dist_build" DIST_BUILD_CONTENT)
foreach(LINE ${DIST_BUILD_CONTENT})
    if(LINE MATCHES "^Container_Bin_Name=\"([^\"]+)\"$")
        set(APP_BIN_NAME ${CMAKE_MATCH_1})
    endif()
endforeach()

# Find dependencies with Conan
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    include("${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
endif()

# Enable Address Sanitizer and Undefined Behavior Sanitizer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")

# Add executable
add_executable(${APP_BIN_NAME} src/main.cpp)
# Future implementation will include utility files
# Will be updated to: add_executable(${APP_BIN_NAME} src/main.cpp src/utils.cpp)

# Link with core library (header-only)
target_link_libraries(${APP_BIN_NAME} INTERFACE cpp_ex_core)

# Link with try_catch_guard if available
if(TRY_CATCH_GUARD_AVAILABLE)
  target_link_libraries(${APP_BIN_NAME} PRIVATE try_catch_guard)
  target_compile_definitions(${APP_BIN_NAME} PRIVATE TRY_CATCH_GUARD_AVAILABLE)

  # Add include directory for the main executable
  target_include_directories(${APP_BIN_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/libs/try_catch_guard/src)
endif()
```

### Catch2 Usage

```cpp
// Include Catch2 headers
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>

// Test case example
TEST_CASE("Vector operations work correctly", "[vector]") {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    SECTION("Vector has the correct size") {
        REQUIRE(v.size() == 5);
    }
    
    SECTION("Vector elements can be accessed") {
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
    }
}
```

### Conan Usage

```bash
# Generate Conan files
conan install .. --output-folder=. --build=missing

# Configure with CMake using Conan toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
```

### Docker Usage

```bash
# Build Docker image
docker build -t "$Container_Name" -f dockerfile.dist .

# Tag Docker image
docker tag "$Container_Name" "$Container_Name:$tag"
```

## Development Workflow

### Local Development

1. Clone the repository
2. Run `build.sh` to build the application without tests (automatically downloads try_catch_guard if needed)
   - Or run `build.sh -test` to build with tests
3. Execute the application from the build directory
4. Make changes to the code
5. Rebuild with `build.sh` or `build.sh -test` as needed

### Planned Utility Components

The project plans to implement utility components in the following files:

- **src/utils.hpp**: Will contain:
  - Utility function declarations
  - Helper class definitions
  - Common constants and type definitions

- **src/utils.cpp**: Will contain:
  - Implementations of utility functions
  - Helper class method implementations

These files are currently in the design phase and will be implemented in the near future.

### Testing Workflow

1. Either:
   - Run `build.sh -test` to build the application with tests
   - Or run `build_test.sh` to build only the tests
2. Run `run_test.sh` to execute all tests (including try_catch_guard tests)
3. Review test results
4. Make changes to fix any failing tests
5. Repeat the process

### Containerization Workflow

1. Update configuration in `.dist_build` and `.env_dist`
2. Run `build.dist.sh` to create a Docker container
3. Test the containerized application
4. Push the container to a registry if needed

### AI Assistant Integration

The project uses Claude AI assistant for documentation maintenance through a custom integration:

1. **Configuration**:
   - The `inject_cline_custom_instructions.sh` script configures Claude with custom instructions
   - SQLite3 is used to store the configuration in VSCode's database
   - jq is used for JSON processing during configuration

2. **Memory Bank Pattern**:
   - Claude maintains a "Memory Bank" of project documentation
   - Core files include projectbrief.md, productContext.md, systemPatterns.md, etc.
   - Documentation follows a clear hierarchy and structure

3. **Documentation Workflow**:
   - Plan Mode: Used for planning and discussing documentation updates
   - Act Mode: Used for implementing documentation changes
   - Documentation updates occur when discovering new patterns, implementing changes, or when explicitly requested

## Deployment Considerations

### Container Configuration

The container is configured with:
- A non-root user (`cpp_ex`)
- Minimal dependencies
- Environment variables from `.env_dist`
- Proper permissions for application directories

### Runtime Environment

The application expects:
- Access to command-line arguments
- Proper environment variables set
- Appropriate permissions for file access

### Testing Environment

The testing environment requires:
- Catch2 testing framework
- try_catch_guard library and its tests
- Address Sanitizer and Undefined Behavior Sanitizer
- Modified signal handling for Catch2
- Proper environment variables for sanitizers

### Versioning

Container images are tagged with:
- `latest` tag
- Custom tags specified in `.dist_build`
- Timestamp-based tags for versioning
