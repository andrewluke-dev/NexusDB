
cmake_minimum_required(VERSION 3.16)

project(NexusDB VERSION 1.0.0 LANGUAGES CXX)

# C++17 configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Database engine library
add_library(nexusdb_lib
    src/Database.cpp
    src/Record.cpp
    src/QueryParser.cpp
    src/Storage.cpp
)

target_include_directories(nexusdb_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(nexusdb_lib PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# Main application
add_executable(nexusdb
    src/main.cpp
)

target_link_libraries(nexusdb
    PRIVATE nexusdb_lib
)

# Automated testing
enable_testing()

add_executable(nexusdb_tests
    tests/test_database.cpp
)

target_link_libraries(nexusdb_tests
    PRIVATE nexusdb_lib
)

add_test(
    NAME NexusDBTests
    COMMAND nexusdb_tests
)
