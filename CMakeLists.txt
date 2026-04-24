cmake_minimum_required(VERSION 3.28)
project(second_laba)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(second_laba
    main.cpp
)

add_executable(tests
    tests/DynamicArrayTests.cpp
    tests/LinkedListTests.cpp
    tests/ArraySequenceTests.cpp
    tests/ListSequenceTests.cpp
    tests/ImmutableArraySequenceTests.cpp
)

target_link_libraries(tests gtest gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)

