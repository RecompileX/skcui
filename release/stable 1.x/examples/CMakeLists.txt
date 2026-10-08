cmake_minimum_required(VERSION 3.20)

project(skcui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(skcui
        full-example.cpp
)