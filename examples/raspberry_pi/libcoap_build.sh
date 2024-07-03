#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

LOG_FILE="build.log"
PROJECT_DIR="../shared/libcoap"
BUILD_DIR="build/rpi"
TOOLCHAIN_FILE="toolchain.cmake"

cd "$PROJECT_DIR"

# Create a toolchain file for cross-compilation
cat <<EOL > "$TOOLCHAIN_FILE"
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_FIND_ROOT_PATH /usr/aarch64-linux-gnu)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
EOL

# If libcoap is not already built, then build else return the path to the static library
if [ ! -f "$BUILD_DIR/include/coap3/coap.h" ] || [ ! -f "$BUILD_DIR/libcoap-3.a" ]; then
    # Remove the existing directory if it exists
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
    fi

    # Create a build directory
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"

    # Configure the build for a static library and append output to log file
    cmake ../.. -DCMAKE_BUILD_TYPE=MinSizeRel \
                -DBUILD_SHARED_LIBS=OFF \
                -DENABLE_DTLS=ON \
                -DWITH_OPENSSL=ON \
                -DWITH_TINYDTLS=OFF \
                -DENABLE_SERVER_MODE=OFF \
                -DENABLE_EXAMPLES=OFF \
                -DENABLE_DOCS=OFF \
                -DCMAKE_TOOLCHAIN_FILE=../../"$TOOLCHAIN_FILE" >> "$LOG_FILE" 2>&1

    # Build the library and append output to the same log file
    make >> "$LOG_FILE" 2>&1

    # Copy the include directory to the build directory
    cp -r ../../include ./

    # Return to the parent directory
    cd ../..
fi

# Echo the path to the static library
echo -n "$(pwd)/$BUILD_DIR"
