//
// Created by Christian Kroer on 8/5/15.
//

#ifndef EFG_SOLVERS_COUNTERFACTUALREGRETMINIMIZATION_H
#define EFG_SOLVERS_COUNTERFACTUALREGRETMINIMIZATION_H


#include "game_solver.h"
#include "../Games/game.h"

class CFR : public GameSolver {
public:
  CFR(const Game &game);
};


#endif //EFG_SOLVERS_COUNTERFACTUALREGRETMINIMIZATION_H
