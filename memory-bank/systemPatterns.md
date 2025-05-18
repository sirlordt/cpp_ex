# System Patterns: C++ Extended Library

## System Architecture

### Overall Architecture
The C++ Extended Library follows a simple, modular architecture designed to demonstrate best practices in C++ development and containerization:

```
┌─────────────────────────────────────────┐
│            C++ Extended Library         │
├─────────────────────────────────────────┤
│                                         │
│  ┌─────────────┐       ┌─────────────┐  │
│  │    Core     │       │ Command-line│  │
│  │ Application │◄─────►│   Parser    │  │
│  │    Logic    │       │             │  │
│  └─────────────┘       └─────────────┘  │
│                                         │
└─────────────────────────────────────────┘
```

### Build System Architecture
The build system follows modern CMake practices with Conan integration:

```
┌─────────────────────────────────────────────────────┐
│                  Build System                       │
├─────────────────────────────────────────────────────┤
│                                                     │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────┐  │
│  │    CMake    │───►│    Conan    │───►│  Build  │  │
│  │ Configuration│    │ Dependencies│    │ Output  │  │
│  └─────────────┘    └─────────────┘    └─────────┘  │
│                                                     │
└─────────────────────────────────────────────────────┘
```

### Containerization Architecture
The containerization process follows a dependency-based approach:

```
┌─────────────────────────────────────────────────────────────────┐
│                  Containerization Process                       │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────────────┐  │
│  │   Build     │───►│ Dependency  │───►│    Dockerfile       │  │
│  │ Application │    │  Analysis   │    │    Generation       │  │
│  └─────────────┘    └─────────────┘    └─────────────────────┘  │
│                                                │                │
│                                                ▼                │
│                                        ┌─────────────────────┐  │
│                                        │    Docker Image     │  │
│                                        │     Building        │  │
│                                        └─────────────────────┘  │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

## Key Technical Decisions

### 1. Modern C++ Standard (C++23)
- **Decision**: Use C++23 as the language standard.
- **Rationale**: Provides access to the latest language features and improvements.
- **Implementation**: Set in CMakeLists.txt with `set(CMAKE_CXX_STANDARD 23)`.

### 2. External Library Integration (try_catch_guard)
- **Decision**: Integrate try_catch_guard library from GitHub.
- **Rationale**: Provides exception handling capabilities and additional testing.
- **Implementation**: Automatic download and integration in build.sh script.

### 3. CMake as Build System
- **Decision**: Use CMake for build configuration.
- **Rationale**: Industry standard, cross-platform, and integrates well with modern C++ workflows.
- **Implementation**: Configured with proper project structure and modern CMake practices.

### 4. Conan for Dependency Management
- **Decision**: Use Conan for managing external dependencies.
- **Rationale**: Provides reproducible builds and simplifies dependency management.
- **Implementation**: Integrated with CMake through the CMakeDeps and CMakeToolchain generators.

### 5. Distrobox for Development Environment
- **Decision**: Use Distrobox to create a consistent development environment.
- **Rationale**: Ensures all developers work with the same tools and dependencies.
- **Implementation**: Configured to provide a complete C++ development environment.

### 6. Minimal Docker Containers
- **Decision**: Generate minimal Docker containers with only required dependencies.
- **Rationale**: Reduces container size and potential security vulnerabilities.
- **Implementation**: Dependency analysis in build.dist.sh to determine required packages.

### 7. Environment Variable Management
- **Decision**: Use .env_dist and .dist_build for environment configuration.
- **Rationale**: Separates configuration from code and allows for different deployment scenarios.
- **Implementation**: Variables processed and expanded during the build process.

### 8. VSCode Debug Configuration
- **Decision**: Implement intelligent launch.json processing during project creation.
- **Rationale**: Ensures proper debugging configuration for each project with minimal manual intervention.
- **Implementation**: Automatically configures program paths in launch.json based on project name.

### 9. Catch2 Testing Framework
- **Decision**: Use Catch2 for testing.
- **Rationale**: Modern, header-only testing framework with good C++ support.
- **Implementation**: Integrated via FetchContent in CMakeLists.txt.

### 10. Sanitizer Integration
- **Decision**: Enable Address Sanitizer and Undefined Behavior Sanitizer.
- **Rationale**: Helps catch memory errors and undefined behavior early.
- **Implementation**: Added compiler and linker flags in CMakeLists.txt.

### 11. Signal Handling Modification
- **Decision**: Modify Catch2's signal handling to work with sanitizers.
- **Rationale**: Prevents conflicts between Catch2's signal handlers and sanitizers.
- **Implementation**: Created modify_catch2.sh script to patch Catch2's signal handling code.

## Design Patterns

### 1. Builder Pattern
- **Usage**: The build.sh, build_test.sh, and build.dist.sh scripts implement a builder pattern.
- **Implementation**: Sequential steps to build the application, tests, and container.

### 2. Factory Pattern
- **Usage**: The CMake configuration acts as a factory for build configurations.
- **Implementation**: Creates different build configurations based on environment.

### 3. Dependency Injection
- **Usage**: External dependencies are injected through Conan.
- **Implementation**: Dependencies declared in conanfile.txt and injected into the build.

### 4. Test Fixture Pattern
- **Usage**: Catch2 test cases implement the test fixture pattern.
- **Implementation**: Setup and teardown handled implicitly by Catch2's SECTION mechanism.

### 5. Decorator Pattern
- **Usage**: Sanitizers decorate the compiled code with runtime checks.
- **Implementation**: Added via compiler and linker flags.

## Critical Implementation Paths

### 1. Build Process
```
1. Check for try_catch_guard library, download if needed
2. Create build directory
3. Generate Conan files
4. Configure with CMake
5. Build the project
```

### 2. Test Build Process
```
1. Check for try_catch_guard library, download if needed
2. Check for required dependencies
3. Create build directory
4. Generate Conan files
5. Configure with CMake
6. Build the test targets (integration_tests and try_catch_guard_tests)
```

### 3. Test Execution Process
```
1. Check for required dependencies
2. Build tests if they don't exist
3. Execute the try_catch_guard tests if available
4. Execute the integration tests
5. Run all tests with CTest
6. Report test results
```

### 4. Containerization Process
```
1. Load variables from configuration files
2. Process environment variables
3. Build application locally
4. Analyze dependencies
5. Generate Dockerfile
6. Build and tag Docker image
```

### 5. Application Execution
```
1. Initialize application
2. Parse command-line arguments
3. Execute main functionality
4. Return exit code
```

## Component Relationships

### Build System Components
- **CMakeLists.txt**: Defines project structure and build configuration
- **conanfile.txt**: Defines external dependencies
- **build.sh**: Automates the build process and try_catch_guard integration
- **build_test.sh**: Automates the test build process

### Testing Components
- **tests/CMakeLists.txt**: Defines test configuration
- **tests/integration_tests.cpp**: Contains test cases
- **modify_catch2.sh**: Modifies Catch2's signal handling
- **run_test.sh**: Automates test execution for all tests
- **external/libs/try_catch_guard**: External library with its own tests

### Containerization Components
- **.dist_build**: Defines container configuration
- **.env_dist**: Defines environment variables for the container
- **build.dist.sh**: Automates the containerization process

### Application Components
- **main.cpp**: Entry point and main application logic
- **Command-line Argument Handling**: Processes user inputs
- **External Libraries**:
  - **try_catch_guard**: Exception handling library from GitHub
- **Planned Utility Components**:
  - **src/utils.hpp**: Will contain utility function declarations and classes
  - **src/utils.cpp**: Will contain utility function implementations
