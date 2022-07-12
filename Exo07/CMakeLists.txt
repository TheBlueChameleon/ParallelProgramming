cmake_minimum_required(VERSION 3.5)

project(Exo7 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Exo7
    main.cpp
    interfaces.hpp util.cpp
    defaultrandgen.hpp defaultrandgen.cpp
    montecarlo.cpp matrices.cpp mandelbrot.cpp
    abstractmatrix.hpp abstractmatrix.cpp
    defaultmatrix.hpp defaultmatrix.cpp
    sparsematrix.hpp sparsematrix.cpp
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Exo7 PUBLIC OpenMP::OpenMP_CXX)
endif()
