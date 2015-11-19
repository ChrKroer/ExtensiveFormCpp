//
// Created by Christian Kroer on 11/18/15.
//

#ifndef EFG_SOLVERS_EGT_H
#define EFG_SOLVERS_EGT_H


#include "game_solver.h"
#include "../Games/game.h"

class EGT : public GameSolver {

public:
  EGT(const Game &game);
  ~EGT();
};


#endif //EFG_SOLVERS_EGT_H
