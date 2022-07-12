cmake_minimum_required(VERSION 3.5)

project(Exo6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Exo6 main.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Exo6 PUBLIC OpenMP::OpenMP_CXX)
endif()
