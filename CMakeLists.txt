cmake_minimum_required(VERSION 3.29)
project(OOD_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(beverages
        main.cpp
        bev.cpp
        Beverages.h
        Condiments.h
        IBeverage.h)
