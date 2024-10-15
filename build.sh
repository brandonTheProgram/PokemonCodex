#!/bin/bash

# Set variables for the project directories
BUILD_DIR="build"

# Check if the build directory exists. If not, create it.
if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
fi

# Navigate to the build directory
cd "$BUILD_DIR"

# Run CMake to configure the project
cmake ..

# Build the project using the number of CPU cores (for faster builds)
cmake --build .

# Go back to the original directory
cd ..