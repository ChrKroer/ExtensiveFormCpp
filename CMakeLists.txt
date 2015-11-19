cmake_minimum_required(VERSION 3.2)
project(efg_solvers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Solvers/game_solver.cpp src/Solvers/game_solver.h src/Games/game.h src/Games/coin.cpp src/Games/coin.h src/Solvers/egt.cpp src/Solvers/egt.h)
add_executable(efg_solvers ${SOURCE_FILES} src/Solvers/cfr.cpp src/Solvers/cfr.h)