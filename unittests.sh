#!/bin/bash

# Set variables for the project directories
BUILD_DIR="build"

# Set the executable name
EXECUTABLE_NAME="UnitTests"

# Check if the build directory exists. If not, warn the user.
if [ -d "$BUILD_DIR" ]; then
  # Navigate to the build directory
  cd "$BUILD_DIR"

  # Check if the executable exists
  if [[ -f "$EXECUTABLE_NAME" ]]; then
    echo "Starting the unit tests"
    ./"$EXECUTABLE_NAME"
  else
    echo "Executable not found. Please make sure the project is built."
  fi
else
  echo "Please make sure the project is built."
fi