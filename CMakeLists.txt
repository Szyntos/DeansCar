cmake_minimum_required(VERSION 3.25)
project(DeansCar)

set(CMAKE_CXX_STANDARD 17)

add_executable(DeansCar main.cpp
        glad/glad.c
        Cell.h
        Grid.cpp
        Cell.cpp
        Direction.h
        CellType.h
        Car.h
        Car.cpp Solver.cpp Solver.h Solver.h AStar.cpp AStar.h AStar.h Move.cpp Move.h)
target_link_libraries(DeansCar -lglfw3)
