cmake_minimum_required(VERSION 3.5)

project(Session10 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Session10 main.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Session10 PUBLIC OpenMP::OpenMP_CXX)
endif()
