cmake_minimum_required(VERSION 3.5)

project(Exo8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Exo8
    main.cpp
    defaultrandgen.hpp defaultrandgen.cpp
    swarm.hpp swarm.cpp
    trajectory.hpp trajectory.cpp
)


find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Exo8 PUBLIC OpenMP::OpenMP_CXX)
endif()
