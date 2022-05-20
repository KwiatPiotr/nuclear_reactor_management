cmake_minimum_required(VERSION 3.20)
project(lab_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab_2 main.cpp)
target_link_libraries(lab_2 /usr/lib/x86_64-linux-gnu/libcrypto++.a)
