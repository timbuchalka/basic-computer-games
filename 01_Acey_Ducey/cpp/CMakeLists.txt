cmake_minimum_required(VERSION 3.30)

# Set compilers before project() command
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm@18/bin/clang")
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm@18/bin/clang++")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable experimental features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexperimental-library -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

# Add each source file as a separate executable
add_executable(AceyDucey main.cpp AceyDucey.cpp)
