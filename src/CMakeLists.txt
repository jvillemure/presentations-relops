cmake_minimum_required(VERSION 3.21)
project(relational-op)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL
	"Enable/Disable output of compile commands during generation."
)

set(CMAKE_CXX_STANDARD 20 CACHE STRING
	"Default value for ``CXX_STANDARD`` target property if set when a target is created"
)

set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL
	"Default value for ``CXX_EXTENSIONS`` target property if set when a target is created."
)

add_executable(canonical-definition-c++20 canonical-definition-c++20.cpp)
add_executable(canonical-definition canonical-definition.cpp)
add_executable(cquantity-bad cquantity-bad.cpp)
add_executable(cquantity-good cquantity-good.cpp)
add_executable(cquantity-good-c++20 cquantity-good-c++20.cpp)
