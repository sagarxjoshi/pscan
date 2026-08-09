#!/bin/sh
set -e

# Define directories cleanly
SOURCE_DIR="."
BUILD_DIR="build-Debug"

echo " Cleaning up older local root files..."
# Remove any accidental files left over in your root directory
rm -f CMakeCache.txt Makefile cmake_install.cmake
rm -rf CMakeFiles/

echo "Step 1: Configuring build files inside target directory..."
# -S points to source code root, -B locks the build artifacts inside the folder
cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug

echo " Step 2: Compiling the binary cleanly..."
# Explicitly builds using the generated path
cmake --build "$BUILD_DIR"

echo "Step 3: Injecting CodeLite Workspace ties..."
# Generates IDE layout cleanly directly targeted at the folder
cmake -G "CodeLite - Unix Makefiles" -S "$SOURCE_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug

echo "Done! All files are locked safely inside the '$BUILD_DIR/' directory."

