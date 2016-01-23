//
// Created by Christian Kroer on 9/1/15.
//

#include "game_solver.h"

efg_solve::GameSolver::GameSolver(Game::SPtr game) : game_(game) {
  average_strategy_[0].resize((unsigned long) game_->num_sequences(Player::P1));
  average_strategy_[1].resize((unsigned long) game_->num_sequences(Player::P2));
}

efg_solve::GameSolver::~GameSolver() {

}

void efg_solve::GameSolver::Combination(double w1, const std::vector<double> &v1,
                                        double w2, std::vector<double> *v2,
                                        Player player) {
  int num_sequences = game_->num_sequences(player);
  for (int i = 0; i < num_sequences; ++i) {
    (*v2)[i] = w1 * v1[i] + w2 * (*v2)[i];
  }
}

void efg_solve::GameSolver::CopyContent(const std::vector<double> &v1, std::vector<double> *v2, Player player) {
  int num_sequences = game_->num_sequences(player);
  for (int i = 0; i < num_sequences; ++i) {
    (*v2)[i] = v1[i];
  }
}
