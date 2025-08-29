#!/bin/bash
set -euo pipefail

echo "[build] Node: $(node -v)  npm: $(npm -v)"

if [ -f "package.json" ]; then
  echo "[build] Installing Node dependencies at repo root..."
  npm ci || npm install
else
  echo "[build] ERROR: No package.json at repo root."
  exit 1
fi

# Set variables for the project directories
BUILD_DIR="build"

# Check if the build directory exists. If not, create it. If it does, clean than make it
if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
else
  ./clean.sh
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