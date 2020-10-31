cmake_minimum_required (VERSION 3.15)
project (TriangleIntersection)

set (CMAKE_CXX_STANDARD 14)

add_executable (
        TriangleIntersection1
        main.cpp
        Geometry.cpp
        Test/TestGenerator.cpp
        GeometryHeader.h
        Algorithm.cpp
        AlgorithmHeader.h
        Test/TestHeader.h
)