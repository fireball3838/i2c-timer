cmake_minimum_required(VERSION 3.5)

project(I2C_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(I2C_tests main.cpp)

target_link_libraries(
    I2C_tests
    -no-pie
    )
