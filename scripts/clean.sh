#!/bin/bash

# Get the absolute path of the script's directory
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
# Navigate to the project root (assumed to be one level above the script's location)
cd "$SCRIPT_DIR/.." || exit


# Ensure the script is being run in the project root directory
BUILD_DIR="build"

# Clean the build directory
rm -rf "$BUILD_DIR"
