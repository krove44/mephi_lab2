cmake_minimum_required(VERSION 3.28)
project(second_laba)

set(CMAKE_CXX_STANDARD 20)

add_executable(second_laba
        Sequence/ListSequence.h
        Sequence/ArraySequence.h
        DynamicArray.h
        LinkedList.h
        main.cpp
        )
