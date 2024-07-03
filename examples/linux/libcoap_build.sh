#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

LOG_FILE="build.log"
PROJECT_DIR="../shared/libcoap"
BUILD_DIR="build/linux"

cd "$PROJECT_DIR"

# if libcoap is not already built, then build else return the path to the static library
if [ ! -f "$BUILD_DIR/include/coap3/coap.h" ] || [ ! -f "$BUILD_DIR/libcoap-3.so" ]; then
    # Remove the existing directory if it exists
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
    fi

    # Create a build directory
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"

    # Configure the build for a shared library and append output to log file
    cmake ../.. -DCMAKE_BUILD_TYPE=MinSizeRel -DBUILD_SHARED_LIBS=ON -DENABLE_DTLS=ON -DWITH_OPENSSL=ON -DENABLE_SERVER_MODE=OFF -DENABLE_EXAMPLES=OFF -DENABLE_DOCS=OFF >> "$LOG_FILE" 2>&1
    # Build the library and append output to the same log file
    make >> "$LOG_FILE" 2>&1

    # Copy the include directory to the build directory
    cp -r ../../include ./

    # Return to the parent directory
    cd ../..
fi

# Echo the path to the static library
echo -n "$(pwd)/$BUILD_DIR"