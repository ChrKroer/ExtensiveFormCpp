//
// Created by Christian Kroer on 11/18/15.
//

#ifndef EFG_SOLVERS_EGT_H
#define EFG_SOLVERS_EGT_H


#include "../Games/game.h"
#include "game_solver.h"

class EGT : public GameSolver {

public:
  enum Prox { kEntropy = 0, kEuclidean = 1 };

  EGT(const Game &game, EGT::Prox prox);
  ~EGT();

private:
  void UpdateInfoSet(int infoset);



  Prox prox;
};


#endif //EFG_SOLVERS_EGT_H
