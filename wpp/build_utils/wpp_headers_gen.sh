#!/bin/bash

# Assign arguments to variables for clarity
SOURCE_DIR="$1"
DESTINATION_DIR="$2"

if [ -d "${DESTINATION_DIR}" ]; then
    rm -rf "${DESTINATION_DIR}"
fi

mkdir -p "${DESTINATION_DIR}"
# Copy wpp/ to headers/
cp -r "${SOURCE_DIR}/" "${DESTINATION_DIR}/wpp/"
mv "${DESTINATION_DIR}/wpp/registry/objects/"* "${DESTINATION_DIR}"
# Copy all .h files to destination directory
find "${DESTINATION_DIR}/wpp" -name "*.h" -exec cp {} "${DESTINATION_DIR}/" \;
# Remove wpp/
rm -rf "${DESTINATION_DIR}/wpp"
# Copy 2305-Wakaama/include/ to destination directory
cp -r "${SOURCE_DIR}/../2305-Wakaama/include/"* "${DESTINATION_DIR}"
# Remove all non-header files from destination directory
find "${DESTINATION_DIR}/" -type f ! -name "*.h" -exec rm {} +