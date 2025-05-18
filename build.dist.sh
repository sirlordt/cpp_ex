#!/bin/bash
# build.dist.sh — Script for building a Docker container from the C++ Extended Library

set -e

echo "Generating Dockerfile for C++ Extended Library..."

# 1. Load variables from .dist_build
if [ -f .dist_build ]; then
    source .dist_build
else
    echo "Error: .dist_build file not found."
    exit 1
fi

# 2. Default values
Container_Group="${Container_Group:-cpp_ex}"
Container_User="${Container_User:-cpp_ex}"
Container_Group_Id="${Container_Group_Id:-1000}"
Container_User_Id="${Container_User_Id:-1000}"

# 3. Generate date/time components
YYYY=$(date "+%Y")
YY=$(date "+%y")
MM=$(date "+%m")
DD=$(date "+%d")
HH=$(date "+%H")
MIN=$(date "+%M")
SS=$(date "+%S")
Z=$(date "+%z")
TIMESTAMP="${YYYY}-${MM}-${DD}-${HH}-${MIN}-${SS}_${Z}"

export YYYY YY MM DD HH MIN SS Z TIMESTAMP

echo "Processing date/time placeholders..."
echo "YYYY: $YYYY, YY: $YY, MM: $MM, DD: $DD"
echo "HH: $HH, MIN: $MIN, SS: $SS, Z: $Z"
echo "TIMESTAMP: $TIMESTAMP"

# 4. Create temporary files with expanded values
echo "Creating temporary files with expanded values..."

# Process .dist_build to .build_env.temp
# Handle the ambiguity between ${MM} for month and minutes
# First, replace the timestamp pattern with a temporary placeholder
cat .dist_build | sed \
    -e "s/\${YYYY}-\${MM}-\${DD}-\${HH}-\${MM}-\${SS}_\${Z}/$TIMESTAMP/g" \
    > .build_env.temp.1

# Then replace individual placeholders
cat .build_env.temp.1 | sed \
    -e "s/\${YYYY}/$YYYY/g" \
    -e "s/\${YY}/$YY/g" \
    -e "s/\${MM}/$MM/g" \
    -e "s/\${DD}/$DD/g" \
    -e "s/\${HH}/$HH/g" \
    -e "s/\${SS}/$SS/g" \
    -e "s/\${Z}/$Z/g" \
    > .build_env.temp.2

# Export variables for envsubst
export Container_Bin_Name="cpp_ex"
# Ensure Container_Bin_Path doesn't have a trailing slash to avoid double slashes
export Container_Bin_Path="/usr/local/bin/cpp_ex"

# Expand remaining variables
cat .build_env.temp.2 | envsubst > .build_env.temp

# Clean up temporary files
rm .build_env.temp.1 .build_env.temp.2

# Process .env_dist to .env_dist.temp
# Handle the ambiguity between ${MM} for month and minutes
# First, replace the timestamp pattern with a temporary placeholder
cat .env_dist | sed \
    -e "s/\${YYYY}-\${MM}-\${DD}-\${HH}-\${MM}-\${SS}_\${Z}/$TIMESTAMP/g" \
    > .env_dist.temp.1

# Then replace individual placeholders
cat .env_dist.temp.1 | sed \
    -e "s/\${YYYY}/$YYYY/g" \
    -e "s/\${YY}/$YY/g" \
    -e "s/\${MM}/$MM/g" \
    -e "s/\${DD}/$DD/g" \
    -e "s/\${HH}/$HH/g" \
    -e "s/\${SS}/$SS/g" \
    -e "s/\${Z}/$Z/g" \
    > .env_dist.temp.2

# Expand remaining variables
cat .env_dist.temp.2 | envsubst > .env_dist.temp

# Clean up temporary files
rm .env_dist.temp.1 .env_dist.temp.2

# Source the temporary files to get expanded values
source .build_env.temp
export Container_Tags Container_App_Folders

# 5. Export variables for envsubst
export Container_Bin_Name Container_Bin_Path

# 6. Process paths and tags
PROCESSED_CONTAINER_BIN_PATH=$(echo "$Container_Bin_Path" | envsubst)
PROCESSED_CONTAINER_TAGS=$(echo "$Container_Tags" | envsubst)

# Ensure Container_Name is not empty
if [ -z "$Container_Name" ]; then
    Container_Name="cpp_ex"
    echo "Warning: Container_Name not set in .dist_build, using default: $Container_Name"
fi

# Ensure we have at least one tag
if [ -z "$PROCESSED_CONTAINER_TAGS" ]; then
    PROCESSED_CONTAINER_TAGS="latest"
    echo "Warning: No tags defined, using default tag: latest"
fi

# Process app folders
PROCESSED_CONTAINER_APP_FOLDERS_RAW=$(echo "$Container_App_Folders" | envsubst)
IFS=';' read -ra PROCESSED_APP_FOLDERS <<< "$PROCESSED_CONTAINER_APP_FOLDERS_RAW"

echo "Container Name: $Container_Name"
echo "Binary Path: $PROCESSED_CONTAINER_BIN_PATH"
echo "Tags: $PROCESSED_CONTAINER_TAGS"
echo "User: $Container_User ($Container_User_Id), Group: $Container_Group ($Container_Group_Id)"
echo "Additional App Folders:"
printf '  - %s\n' "${PROCESSED_APP_FOLDERS[@]}"

# 7. Process .env_dist.temp
echo "Processing .env_dist.temp file..."
ENV_VARS=""
if [ -f .env_dist.temp ]; then
    while IFS= read -r line || [[ -n "$line" ]]; do
        [[ -z "$line" || "$line" =~ ^# ]] && continue
        key=${line%%=*}
        raw_value=${line#*=}
        expanded_value=$(echo "$raw_value" | envsubst)
        ENV_VARS+="ENV $key=$expanded_value"$'\n'
    done < .env_dist.temp
else
    echo "Warning: .env_dist.temp file not found."
fi

# 8. Build locally with Conan + CMake
echo "Building project with Conan and CMake..."
./build.sh

# 9. Detect dependencies
echo "Detecting shared library dependencies..."
# Start with basic packages that are always needed
PACKAGES="ca-certificates"
CONTAINER_DEPS=""

# Map of known libraries to their packages
declare -A LIB_TO_PKG_MAP
LIB_TO_PKG_MAP["/lib/x86_64-linux-gnu/libasan.so.6"]="libasan6"
LIB_TO_PKG_MAP["/lib/x86_64-linux-gnu/libstdc++.so.6"]="libstdc++6"
LIB_TO_PKG_MAP["/lib/x86_64-linux-gnu/libubsan.so.1"]="libubsan1"
LIB_TO_PKG_MAP["/lib/x86_64-linux-gnu/libm.so.6"]="libc6"  # libm is part of libc6

# Function to add a package to our list
add_package() {
    local pkg="$1"
    
    # Check if package is already in our list
    if [[ ! " $PACKAGES " =~ " $pkg " ]]; then
        echo "Adding package: $pkg"
        PACKAGES="$PACKAGES $pkg"
        
        # Get package version
        PKG_VERSION=$(dpkg-query -W -f='${Version}' "$pkg" 2>/dev/null)
        if [ -n "$PKG_VERSION" ]; then
            if [ -n "$CONTAINER_DEPS" ]; then
                CONTAINER_DEPS="$CONTAINER_DEPS;"
            fi
            CONTAINER_DEPS="${CONTAINER_DEPS}${pkg}:${PKG_VERSION}"
        else
            echo "Warning: Could not determine version for package: $pkg"
        fi
    fi
}

# Use ldd to detect shared library dependencies
if [ -f "build/$Container_Bin_Name" ]; then
    echo "Analyzing executable: build/$Container_Bin_Name"
    
    # Get all shared libraries used by the executable
    LIBS=$(ldd "build/$Container_Bin_Name" | grep "=>" | awk '{print $3}' | sort | uniq)
    
    echo "Found shared libraries:"
    echo "$LIBS"
    
    # Find which Debian packages provide these libraries
    for lib in $LIBS; do
        # Skip libraries that don't exist (like linux-vdso.so.1)
        if [ ! -f "$lib" ]; then
            continue
        fi
        
        # Check if we have this library in our known map
        if [ -n "${LIB_TO_PKG_MAP[$lib]}" ]; then
            pkg="${LIB_TO_PKG_MAP[$lib]}"
            echo "Library $lib provided by package (from map): $pkg"
            add_package "$pkg"
        else
            # Find the package that provides this library
            PKG=$(dpkg -S "$lib" 2>/dev/null | cut -d: -f1 | sort | uniq)
            
            if [ -n "$PKG" ]; then
                for pkg in $PKG; do
                    echo "Library $lib provided by package: $pkg"
                    add_package "$pkg"
                done
            else
                echo "Warning: Could not determine package for library: $lib"
                
                # Try to guess based on filename
                base_name=$(basename "$lib")
                if [[ "$base_name" =~ ^lib([^.]+)\.so\. ]]; then
                    lib_name="${BASH_REMATCH[1]}"
                    potential_pkg="lib${lib_name}"
                    
                    # Check if this package exists
                    if dpkg -l "$potential_pkg" &>/dev/null; then
                        echo "Guessing package for $lib: $potential_pkg"
                        add_package "$potential_pkg"
                    else
                        echo "Could not guess package for $lib"
                    fi
                fi
            fi
        fi
    done
    
    # Always add these critical packages for C++ applications
    add_package "libc6"
    add_package "libgcc-s1"
    add_package "libstdc++6"
else
    echo "Warning: Executable not found at build/$Container_Bin_Name"
    echo "Using default dependencies: libc6 libgcc-s1 libstdc++6"
    add_package "libc6"
    add_package "libgcc-s1"
    add_package "libstdc++6"
fi

echo "Detected packages: $PACKAGES"
echo "Container dependencies: $CONTAINER_DEPS"

# 10. Generate dockerfile.dist
echo "Generating dockerfile.dist..."
cat > dockerfile.dist <<EOF
# Generated by build.dist.sh on $(date)
FROM ubuntu:22.04

LABEL description="$Container_Description"
LABEL maintainer="$Container_Mantainer"
LABEL tags="$PROCESSED_CONTAINER_TAGS"
LABEL path="$PROCESSED_CONTAINER_BIN_PATH"

RUN apt-get update && \\
    apt-get install -y --no-install-recommends $PACKAGES && \\
    apt-get clean && rm -rf /var/lib/apt/lists/*

$ENV_VARS
ENV APP_DEPENDENCIES="$CONTAINER_DEPS"

RUN groupadd -g $Container_Group_Id $Container_Group && \\
    useradd -m -u $Container_User_Id -g $Container_Group -s /bin/bash $Container_User && \\
    mkdir -p $PROCESSED_CONTAINER_BIN_PATH && \\
EOF

# Add app folders
for folder in "${PROCESSED_APP_FOLDERS[@]}"; do
  echo "    mkdir -p \"$folder\" && chown $Container_User:$Container_Group \"$folder\" && \\" >> dockerfile.dist
done

# Copy binary and set permissions
cat >> dockerfile.dist <<EOF
    chown -R $Container_User:$Container_Group $PROCESSED_CONTAINER_BIN_PATH

COPY build/$Container_Bin_Name $PROCESSED_CONTAINER_BIN_PATH/$Container_Bin_Name
RUN chown $Container_User:$Container_Group $PROCESSED_CONTAINER_BIN_PATH/$Container_Bin_Name && \\
    chmod 755 $PROCESSED_CONTAINER_BIN_PATH/$Container_Bin_Name

USER $Container_User
WORKDIR $PROCESSED_CONTAINER_BIN_PATH
ENTRYPOINT ["$PROCESSED_CONTAINER_BIN_PATH/$Container_Bin_Name"]
EOF

# 11. Build and tag
echo "Dockerfile generated: dockerfile.dist"
docker build -t "$Container_Name" -f dockerfile.dist .

# Use semicolons as the delimiter for Container_Tags
IFS=';' read -ra TAG_ARRAY <<< "$PROCESSED_CONTAINER_TAGS"
for tag in "${TAG_ARRAY[@]}"; do
    clean_tag=$(echo "$tag" | xargs)
    if [ -n "$clean_tag" ]; then
        echo "Tagging image: $Container_Name:$clean_tag"
        docker tag "$Container_Name" "$Container_Name:$clean_tag"
    fi
done

echo "Docker image '$Container_Name' built successfully!"

# Debug information
echo "Temporary files created for debugging:"
echo "- .build_env.temp: Expanded values from .dist_build"
echo "- .env_dist.temp: Expanded values from .env_dist"
echo "These files are kept for debugging purposes."
