cmake_minimum_required(VERSION 3.5)

project(Session11 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Session11 main.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Session11 PUBLIC OpenMP::OpenMP_CXX)
endif()
