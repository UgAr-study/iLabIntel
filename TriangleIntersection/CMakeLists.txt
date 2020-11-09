cmake_minimum_required (VERSION 3.15)
project (TriangleIntersection)

set (CMAKE_CXX_STANDARD 14)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory [-g]")


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