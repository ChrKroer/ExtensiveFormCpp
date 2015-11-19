cmake_minimum_required(VERSION 3.2)
project(efg_solvers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Solvers/GameSolver.cpp src/Solvers/GameSolver.h src/Games/Game.h src/Games/Coin.cpp src/Games/Coin.h)
add_executable(efg_solvers ${SOURCE_FILES} src/Solvers/CounterfactualRegretMinimization.cpp src/Solvers/CounterfactualRegretMinimization.h)