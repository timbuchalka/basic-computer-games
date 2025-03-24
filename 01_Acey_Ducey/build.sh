#!/bin/bash

# Exit on any error
set -e

# Build configuration
BUILD_DIR="cmake-build-debug"
CPP_SUBDIR="cpp"
EXECUTABLE_NAME="AceyDucey"
EXECUTABLE_PATH="$BUILD_DIR/$CPP_SUBDIR/$EXECUTABLE_NAME"

# Optional: Uncomment for clean build
# rm -rf "$BUILD_DIR"

# Create build directory and enter it
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure only if not already done
if [ ! -f CMakeCache.txt ]; then
    echo "🔧 Running CMake configuration..."
    cmake ..
fi

# Build the project
echo "⚙️  Building project..."
if cmake --build .; then
    echo "✅ Build succeeded."
    cd ..

    # Run if binary exists
    if [ -x "$EXECUTABLE_PATH" ]; then
        echo "🚀 Running ./$EXECUTABLE_PATH ..."
        "./$EXECUTABLE_PATH"
    else
        echo "❌ Executable not found at: $EXECUTABLE_PATH"
        echo "🔍 Try running: find $BUILD_DIR -type f -perm +111"
    fi
else
    echo "❌ Build failed. Skipping execution."
    cd ..
fi

echo "🏁 Done."