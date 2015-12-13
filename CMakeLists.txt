cmake_minimum_required(VERSION 3.2)

project(efg_solve)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/solvers/game_solver.cpp src/solvers/game_solver.h
        src/games/game.h
        src/games/coin.cpp src/games/coin.h
        src/solvers/egt.cpp src/solvers/egt.h
        src/solvers/cfr.cpp src/solvers/cfr.h
        src/games/full_game.cpp src/games/full_game.h
        src/solvers/prox.h
        src/solvers/dilated_entropy.cpp src/solvers/dilated_entropy.h src/config.cpp src/config.h src/games/game.cpp)
add_executable(efg_solve ${SOURCE_FILES} src/main.cpp)


#set(GTEST /Users/ckroer/install/c++/googletest/googletest/)
#include_directories(${GTEST}/include ${GTEST})


enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_FILES ${SOURCE_FILES} test/game_test.cpp test/dilated_entropy_test.cpp test/egt_test.cpp)
add_executable(efg_test ${TEST_FILES})
target_link_libraries(efg_test ${GTEST_BOTH_LIBRARIES})


add_test(efg_test game_test)
#target_link_libraries(game_test ${GTEST}/lib/libgtest.a ${GTEST}/lib/libgtest_main.a)



