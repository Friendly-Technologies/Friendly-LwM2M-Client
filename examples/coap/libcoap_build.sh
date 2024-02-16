#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

LOG_FILE="build.log"
BUILD_DIR="build"

cd libcoap

# Remove the existing directory if it exists
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi

# Create a build directory
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure the build for a static library and append output to log file
cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel -DENABLE_DTLS=ON -DENABLE_SERVER_MODE=OFF -DENABLE_EXAMPLES=OFF -DENABLE_DOCS=OFF >> "$LOG_FILE" 2>&1

# Build the library and append output to the same log file
make >> "$LOG_FILE" 2>&1

# The static library will be located in build/src/
LIB_PATH="$(pwd)"
# Echo the path to the static library
echo -n "$LIB_PATH"