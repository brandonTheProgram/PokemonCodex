#!/bin/bash

set -euo pipefail

BUILD_DIR="build"
FRONTEND_PORT="${FRONTEND_PORT:-3000}"
BACKEND_PORT="${BACKEND_PORT:-8080}"
EXECUTABLE_NAME="PokemonCodex"
EXPRESS_SERVER_FILE="Pokedex.js"
EXPRESS_SERVER_LINK="http://localhost:${FRONTEND_PORT}"

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
