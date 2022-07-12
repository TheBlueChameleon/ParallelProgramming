cmake_minimum_required(VERSION 3.5)

project(Session5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Session5 main.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Session5 PUBLIC OpenMP::OpenMP_CXX)
endif()
