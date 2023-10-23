cmake_minimum_required(VERSION 3.10)

project(vcopy VERSION 0.0.1 LANGUAGES CXX)

include(CTest)

add_subdirectory(src)
add_subdirectory(tool)
add_subdirectory(test)
