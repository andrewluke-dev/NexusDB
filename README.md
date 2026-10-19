
# NexusDB

A lightweight database engine built with modern C++17.

NexusDB is a terminal-based database application featuring CRUD operations, indexed record lookup, persistent file storage, and automated testing.

## Features

- Insert, select, update, and delete records
- Fast ID lookup using std::unordered_map
- Persistent local file storage
- SQL-inspired command interface
- Input validation and duplicate ID detection
- Sorted record listing
- CMake build system
- Automated tests with CTest
- C++17 and STL

## Technologies

- C++17
- STL
- CMake
- CTest
- GCC / G++
- Linux / WSL

## Project Structure

    NexusDB/
    ├── include/
    │   ├── Database.hpp
    │   ├── Record.hpp
    │   ├── QueryParser.hpp
    │   └── Storage.hpp
    ├── src/
    │   ├── Database.cpp
    │   ├── Record.cpp
    │   ├── QueryParser.cpp
    │   ├── Storage.cpp
    │   └── main.cpp
    ├── tests/
    │   └── test_database.cpp
    ├── data/
    ├── CMakeLists.txt
    └── README.md

## Build

Requires CMake 3.16+ and a C++17-compatible compiler.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Run

From the project root:

```bash
./build/nexusdb
```

## Available Commands

```text
INSERT 1 "Andrew" "Developer"
SELECT 1
SELECT ALL
UPDATE 1 "Andrew" "C++ Engineer"
DELETE 1
HELP
EXIT
```

## Testing

```bash
ctest --test-dir build --output-on-failure
```

The test executable checks 10 scenarios covering CRUD operations, validation, parsing, storage, and database management.

## Architecture

NexusDB consists of four main components:

- Record: represents a database record.
- Database: manages records and ID indexing.
- QueryParser: interprets terminal commands.
- Storage: handles file persistence.

## Limitations

NexusDB is an educational database engine, not a full SQL database.

It does not currently support SQL joins, transactions, concurrent clients, authentication, or automatic recovery after an unexpected shutdown.

## Author

Andrew Luke

GitHub: https://github.com/andrewluke-dev
