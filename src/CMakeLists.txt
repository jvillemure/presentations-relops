cmake_minimum_required(VERSION 3.20)
project(relational-op)

set(CMAKE_CXX_STANDARD 20)

add_executable(cquantity-bad cquantity-bad.cpp)
add_executable(cquantity-good cquantity-good.cpp)
add_executable(cquantity-c++20 cquantity-c++20.cpp)
add_executable(canonical-definition canonical-definition.cpp)
