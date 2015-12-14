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

    double excessive_gap();
    double gap();
    virtual void Run(int num_iterations);
  private:
    std::vector<double> &best_response(Player player) {if (player == Player::P1) return best_response_[0]; else return best_response_[1];}
    void Init();
    void Iteration(Player player, Player opponent, double tau);
    double mu(Player player) {if (player == Player::P1) return mu_[0]; else return mu_[1]; }



    // The prox operator used for taking steps.
    const Prox *prox_;
    // array for storing the smoothed best response
    std::array<std::vector<double>, 2> best_response_;
    // Intermediate array for Bregman step
    std::vector<double> intermediate_;
    // array for storing expected values of sequences
    std::vector<double> utility_;
    std::array<double, 2> mu_;
  };

}

#endif //EFG_SOLVERS_EGT_H
