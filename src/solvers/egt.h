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
    typedef std::unique_ptr<EGT> UPtr;

    EGT(Game::SPtr game, Prox::SPtr prox);
    EGT(Game::SPtr game, Prox::SPtr prox, double mu, double gamma);
    EGT(Game::SPtr game, Prox::SPtr prox, double mu, double gamma, double threshold);

    ~EGT() override;

    double excessive_gap();
    double excessive_gap(double &ev_x, double &ev_y);
    double gap();
    double gap(double &ev_x, double &ev_y);
    int sequences_touched() const { return sequences_touched_; }
    void Run(int num_iterations) override;

    void WarmStart(std::array<std::vector<double>, 2> strategy_profile);

    const std::array<double, 2> mu() { return mu_; }
  private:
    std::vector<double> &best_response(Player player) {if (player == Player::P1) { return best_response_[0]; } { return best_response_[1];
}}
    void Init();
    void Iteration(Player player, Player opponent, double tau);
    double mu(Player player) {if (player == Player::P1) { return mu_[0]; } { return mu_[1];
}}

    void mu(Player player, double mu) { if (player == Player::P1) { mu_[0] = mu; } else { mu_[1] = mu;
}}


    // The prox operator used for taking steps.
    const Prox::SPtr prox_;
    // array for storing the smoothed best response
    std::array<std::vector<double>, 2> best_response_;
    // Intermediate array for Bregman step
    std::vector<double> intermediate_;
    // array for storing expected values of sequences
    std::vector<double> utility_;
    std::array<double, 2> mu_;
    double gamma_;
    double threshold_;

    int sequences_touched_ = 0;
  };

}  // namespace efg_solve

#endif //EFG_SOLVERS_EGT_H
