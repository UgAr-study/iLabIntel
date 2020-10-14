cmake_minimum_required (VERSION 3.17)
project (TriangleIntersection)

set (CMAKE_CXX_STANDARD 14)

find_package (GTest REQUIRED)
find_package (Threads REQUIRED)

include_directories (
        ${GTEST_INCLUDE_DIRS}
)

add_executable(
        RandTest
        Test/TestGenerator.cpp
        Geometry.cpp
        GeometryHeader.h
        Algorithm.cpp
        AlgorithmHeader.h
)

add_executable (
        TriangleIntersection
        main.cpp
        Geometry.cpp
        GeometryHeader.h
        Algorithm.cpp
        AlgorithmHeader.h
)

add_executable (
        GoogleTestGeometry
        Test/test_tr.cpp
        Geometry.cpp
        GeometryHeader.h
        Test/TestGenerator.cpp
)

target_link_libraries ( GoogleTestGeometry ${GTEST_LIBRARIES} Threads::Threads )