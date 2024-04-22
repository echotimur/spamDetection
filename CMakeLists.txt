cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(antispam)

set(CMAKE_CXX_STANDARD 17)

include_directories(../../../library/boost_1_82_0)
add_executable(learning learning.cpp)
