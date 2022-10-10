cmake_minimum_required(VERSION 3.5)

project(for_dr_web LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(for_dr_web main.cpp dictionary.h)
