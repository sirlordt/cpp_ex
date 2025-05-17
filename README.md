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

- **Development Tools Available**:
  - **Build Tools**: build-essential, CMake 3.28.3
  - **Package Manager**: Conan for C++ dependencies
  - **Version Control**: git for source control
  - **Utilities**: mc (Midnight Commander), htop for system monitoring
  - **IDE**: Visual Studio Code with C++ extensions
  - **Containerization**: Docker Engine and Docker Compose for building and running containers

- **Project Structure**:
  - Modern CMake project layout
  - Proper separation of build configuration
  - Environment variable management
  - Containerization support

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
