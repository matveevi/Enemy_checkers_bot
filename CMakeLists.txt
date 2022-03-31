cmake_minimum_required(VERSION 3.0)

project(MyMainProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(MAIN_SOURCE src/main.cpp)

add_executable(MyMainProject ${MAIN_SOURCE})