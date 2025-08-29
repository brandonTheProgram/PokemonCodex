#!/bin/bash

set -euo pipefail

# Set variables for the project directories
BUILD_DIR="build"

# Set the executable name
EXECUTABLE_NAME="PokemonCodex"
EXPRESS_SERVER_FILE="Pokedex.js"
EXPRESS_SERVER_LINK="http://localhost:3000"

# Install Node deps if missing (idempotent)
if [ -f "express/package.json" ]; then
  echo "Installing Node dependencies in ./express"
  npm --prefix express ci --omit=dev
fi

# Check if the build directory and executable exists. If not, warn the user.
if [ -d "$BUILD_DIR" ] && [ -f "$BUILD_DIR/$EXECUTABLE_NAME" ]; then
  echo "Starting the backend server"
  "./$BUILD_DIR/$EXECUTABLE_NAME" &

  # Start the Express server
  echo "Starting the frontend server"
  cd express
  exec node "$EXPRESS_SERVER_FILE"

  echo "Both servers are running. Access the webpage at ${EXPRESS_SERVER_LINK}"

else
  echo "Executable not found. Please make sure the project is built."
fi
