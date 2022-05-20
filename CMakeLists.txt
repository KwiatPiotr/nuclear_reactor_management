cmake_minimum_required(VERSION 3.20)
project(NRMS)

set(CMAKE_CXX_STANDARD 20)

add_executable(NRMS main.cpp)
target_link_libraries(NRMS /usr/lib/x86_64-linux-gnu/libcrypto++.a)
