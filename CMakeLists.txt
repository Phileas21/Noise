cmake_minimum_required(VERSION 3.14)
project(Graph)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(Graph main.cpp)

target_link_libraries(Graph sfml-graphics)