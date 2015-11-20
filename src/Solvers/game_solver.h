//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_GAMESOLVER_H
#define EFG_SOLVERS_GAMESOLVER_H


#include "../Games/game.h"

class GameSolver {
public:
  GameSolver(Game game);
  ~GameSolver();

  virtual void solve() = 0;

  const double* &getStrategyP1();
  const double* &getStrategyP2();
protected:
  Game game;

  double strategyP1[];
  double strategyP2[];
};


#endif //EFG_SOLVERS_GAMESOLVER_H
