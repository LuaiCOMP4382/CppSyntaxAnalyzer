cmake_minimum_required(VERSION 3.7)

project(SyntaxAnalyzer)

add_executable(SyntaxAnalyzer src/main.cpp src/util.cpp src/token.cpp src/lexical_analyzer.cpp src/syntax_parser.cpp)
