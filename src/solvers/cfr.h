//
// Created by Christian Kroer on 8/5/15.
//

#ifndef EFG_SOLVERS_COUNTERFACTUALREGRETMINIMIZATION_H
#define EFG_SOLVERS_COUNTERFACTUALREGRETMINIMIZATION_H


#include "game_solver.h"
#include "../Games/game.h"

namespace efg_solve {
  class CFR : public GameSolver {
  public:
    CFR(Game *game);
  };
}

#endif //EFG_SOLVERS_COUNTERFACTUALREGRETMINIMIZATION_H
