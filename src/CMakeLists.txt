cmake_minimum_required(VERSION 3.16)
project(ADA_PF_GARCIA_JIMENEZ_USUGA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ada_pf
        main.cpp
        parser.cpp
        mergeSort.cpp
        binarySearch.cpp
        graph.cpp
        kruskal.cpp)
