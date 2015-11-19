//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_GAMESOLVER_H
#define EFG_SOLVERS_GAMESOLVER_H


class GameSolver {
public:
  GameSolver(Game game);
  ~GameSolver();

  const double* &getStrategyP1();
  const double* &getStrategyP2();
protected:
  Game game;

  double strategyP1[];
  double strategyP2[];
};


#endif //EFG_SOLVERS_GAMESOLVER_H
