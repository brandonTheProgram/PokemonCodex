#!/bin/bash

# Set variables for the project directories
BUILD_DIR="build"

# Set the executable name
EXECUTABLE_NAME="PokemonCodex"
EXPRESS_SERVER_FILE="Pokedex.js"
EXPRESS_SERVER_LINK="http://localhost:3000"

# Check if the build directory exists. If not, warn the user.
if [ -d "$BUILD_DIR" ]; then
  # Navigate to the build directory
  cd "$BUILD_DIR"

  # Check if the executable exists
  if [[ -f "$EXECUTABLE_NAME" ]]; then
    # Run the backend server
    echo "Starting the backend server"
    ./"$EXECUTABLE_NAME" &

    # Start the Express server
    cd ..
    cd express
    node "$EXPRESS_SERVER_FILE" &

    # Wait for both servers to be ready
    sleep 2

    echo "Both servers are running. Access the webpage at ${EXPRESS_SERVER_LINK}"
    cd ..
  else
    echo "Executable not found. Please make sure the project is built."
  fi
else
  echo "Please make sure the project is built."
fi