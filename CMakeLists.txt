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

add_executable(race_test
    src/race_test.cpp
    src/logger.cpp
)

add_executable(crash_test
    src/crash_test.cpp
)

add_executable(race_replay_test
    src/race_replay_test.cpp
)