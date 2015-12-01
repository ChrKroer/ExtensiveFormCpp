//
// Created by Christian Kroer on 11/18/15.
//

#ifndef EFG_SOLVERS_EGT_H
#define EFG_SOLVERS_EGT_H


#include "../Games/game.h"
#include "game_solver.h"
#include "prox.h"

namespace efg_solve {

  class EGT : public GameSolver {

  public:

    EGT(Game *game, Prox *prox);

    virtual ~EGT();

  private:
    void Iterate(int num_iterations);
    std::vector<double> &best_response(Player player) {if (player == Player::P1) return best_response_[0]; else return best_response_[1];}
    // The prox operator used for taking steps.
    const Prox *prox_;
    // array for storing the smoothed best response
    std::vector<double> best_response_[2];
    // Intermediate array for Bregman step
    std::vector<double> intermediate_;
    // array for storing expected values of sequences
    std::vector<double> utility_;
    double mu_[2];
  };

}

#endif //EFG_SOLVERS_EGT_H
