//
// Created by Christian Kroer on 11/18/15.
//

#ifndef EFG_SOLVERS_EGT_H
#define EFG_SOLVERS_EGT_H


#include "../Games/game.h"
#include "game_solver.h"
#include "Prox.h"

class EGT : public GameSolver {

public:

  EGT(const Game &game, EGT::Prox prox);
  ~EGT();

private:
  void Iterate(int num_iterations);



  Prox prox;
};


#endif //EFG_SOLVERS_EGT_H
