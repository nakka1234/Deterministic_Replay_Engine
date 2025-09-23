cmake_minimum_required(VERSION 3.10)
project(test_program)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include headers
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Build the test program
add_executable(testprog
    src/test_program.cpp
    src/logger.cpp
)
