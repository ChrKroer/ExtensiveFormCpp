//
// Created by Christian Kroer on 9/1/15.
//

#include "game_solver.h"

efg_solve::GameSolver::GameSolver(Game *game) : game_(game) {
  average_strategy_[0].resize((unsigned long) game_->num_sequences(Player::P1));
  average_strategy_[1].resize((unsigned long) game_->num_sequences(Player::P2));

  current_strategy_[0].resize((unsigned long) game_->num_sequences(Player::P1));
  current_strategy_[1].resize((unsigned long) game_->num_sequences(Player::P2));
}

efg_solve::GameSolver::~GameSolver() {

}

void efg_solve::GameSolver::ConvexCombination(std::vector<double> &v1, std::vector<double> *v2, double stepsize, Player player) {
  int num_sequences = game_->num_sequences(player);
  for (int i = 0; i < num_sequences; ++i) {
    (*v2)[i] = stepsize * v1[i] + (1 - stepsize) * (*v2)[i];
  }
}
