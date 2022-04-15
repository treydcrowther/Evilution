cmake_minimum_required(VERSION 3.20)
project(Evilution)

set(CMAKE_CXX_STANDARD 14)

# Add MPI Package to Project
find_package(MPI REQUIRED)

add_executable(Evilution
        src/main.cpp
        src/Organism.cpp src/Organism.h
        src/Board.cpp src/Board.h
        src/Simulation.cpp src/Simulation.h
        src/Configuration.cpp src/Configuration.h)

# Add libraries for code completion and compiling
target_link_libraries(Evilution PUBLIC MPI::MPI_CXX)
