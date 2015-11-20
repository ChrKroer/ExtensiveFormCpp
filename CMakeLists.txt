cmake_minimum_required(VERSION 3.2)

project(efg_solve)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/main.cpp
        src/Solvers/game_solver.cpp src/Solvers/game_solver.h
        src/Games/game.h
        src/Games/coin.cpp src/Games/coin.h
        src/Solvers/egt.cpp src/Solvers/egt.h
        src/Solvers/cfr.cpp src/Solvers/cfr.h
        test/game_test.cpp src/Games/FullGame.cpp src/Games/FullGame.h)
add_executable(efg_solve ${SOURCE_FILES} )


set(GTEST /Users/ckroer/install/c++/googletest/googletest/)
include_directories(${GTEST}/include ${GTEST})

set(TEST_FILES ${SOURCE_FILES} test/game_test.cpp src/Games/FullGame.cpp src/Games/FullGame.h)
add_executable(gameTest ${TEST_FILES})

target_link_libraries(gameTest gtest gtest_main)
