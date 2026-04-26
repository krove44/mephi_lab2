cmake_minimum_required(VERSION 3.28)
project(second_laba)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(second_laba
    main.cpp
)

find_path(NCURSESW_INCLUDE_DIR
    NAMES ncursesw/curses.h
    PATHS C:/mingw64/opt/include
)

find_library(NCURSESW_LIBRARY
    NAMES ncursesw libncursesw.a
    PATHS C:/mingw64/opt/lib
)

target_include_directories(second_laba PRIVATE ${NCURSESW_INCLUDE_DIR})
target_link_libraries(second_laba PRIVATE ${NCURSESW_LIBRARY})

add_executable(tests
    tests/DynamicArrayTests.cpp
    tests/LinkedListTests.cpp
    tests/ArraySequenceTests.cpp
    tests/ListSequenceTests.cpp
    tests/ImmutableArraySequenceTests.cpp
)

target_link_libraries(tests PRIVATE gtest gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)
