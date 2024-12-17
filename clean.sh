#!/bin/bash

# Define the build directory to be deleted
BUILD_DIR="build"

# Check if the build directory exists
if [ -d "$BUILD_DIR" ]; then
    echo "Removing build directory: $BUILD_DIR"
    rm -rf "$BUILD_DIR"  # Use -rf to recursively delete the directory and its contents
    echo "Build directory removed."
else
    echo "Build directory does not exist."
fi