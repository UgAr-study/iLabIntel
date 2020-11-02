cmake_minimum_required(VERSION 3.15)
project(ParaCL)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        ParaCL
        main.cpp

        src/Parser.cpp
        src/Expression.cpp
        src/Lexem.cpp
        src/Methods.cpp

        include/ParserHeader.h
        include/ExpressionHeader.h
        include/LexemHeader.h
        include/MainHeader.h
)