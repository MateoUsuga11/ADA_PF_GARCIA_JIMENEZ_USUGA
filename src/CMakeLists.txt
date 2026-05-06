cmake_minimum_required(VERSION 4.2)
project(src)

set(CMAKE_CXX_STANDARD 20)

add_executable(src Parser.cpp
        mergeSort.cpp
        binarySearch.cpp
        Parser.hpp
        mergeSort.hpp
        binarySearch.hpp
        main.cpp)
