//
// Created by Christian Kroer on 11/18/15.
//

#include <cmath>
#include "egt.h"
#include "../config.h"

efg_solve::EGT::EGT(Game::SPtr game, Prox::SPtr prox) : EGT(game, prox, config::mu, config::gamma) {

}


efg_solve::EGT::EGT(Game::SPtr game, Prox::SPtr prox, double mu, double gamma) : GameSolver(std::move(game)), prox_(std::move(prox)) {
  mu_[0] = mu;
  mu_[1] = 1.0/gamma;
  Init();
}

efg_solve::EGT::~EGT() {

}

double efg_solve::EGT::excessive_gap() {
  game_->UtilityVector(&average_strategy(Player::P1), &utility_, Player::P2);
  prox_->ProxStep(-1 / mu(Player::P2), Player::P2, &utility_, &best_response(Player::P2));
  // ProxStep converts to a minimization problem for P2, so we need to negate the return value.
  double smooth_br_val_f = -utility_[0] * mu(Player::P2);

  game_->UtilityVector(&average_strategy(Player::P2), &utility_, Player::P1);
  prox_->ProxStep(-1 / mu(Player::P1), Player::P1, &utility_, &best_response(Player::P1));
  // ProxStep converts to a minimization problem for P1, which is what we want.
  double smooth_br_val_phi = utility_[0] * mu(Player::P1);

  return smooth_br_val_phi - smooth_br_val_f;
}

double efg_solve::EGT::gap(){
  game_->UtilityVector(&average_strategy(Player::P1), &utility_, Player::P2);
  double br_val_f = game_->BestResponseValue(Player::P2, &utility_);
  game_->UtilityVector(&average_strategy(Player::P2), &utility_, Player::P1);
  double br_val_phi = -game_->BestResponseValue(Player::P1, &utility_);
  return br_val_f - br_val_phi;
};

void efg_solve::EGT::Init() {
  unsigned int max_sequences = (unsigned int) std::max(game_->num_sequences(Player::P1), game_->num_sequences(Player::P2));
  best_response_[0].resize((unsigned long) game_->num_sequences(Player::P1), 0);
  best_response_[1].resize((unsigned long) game_->num_sequences(Player::P2), 0);
  intermediate_.resize(max_sequences, 0);
  utility_.resize(max_sequences, 0);

  game_->InitUniform(&average_strategy(Player::P1), Player::P1);
  average_strategy(Player::P1)[0] = 1;
  average_strategy(Player::P2)[0] = 1;
  // prox step P2
  game_->UtilityVector(&average_strategy(Player::P1), &utility_, Player::P2);
  prox_->ProxStep(-1 / mu(Player::P2), Player::P2, &utility_, &average_strategy(Player::P2));

  // Bregman projection P1
  game_->UtilityVector(&best_response(Player::P2), &utility_, Player::P1);
  prox_->BregmanProjection(-config::gamma, Player::P1, &average_strategy(Player::P1), &utility_,
                           &average_strategy(Player::P1));

//  printf("\ninitial gap           = %0.04lf\n", gap());
//  printf("initial excessive gap = %0.04lf\n", excessive_gap());
};

void efg_solve::EGT::Run(int num_iterations) {
  for (int iterate = 0; iterate < num_iterations; ++iterate) {
    double tau = 2.0 / (iterations_ + 3);

    if ((iterations_ % 2) == 0) {
      Iteration(Player::P1, Player::P2, tau);
      mu_[0] = (1 - tau) * mu_[0];
    } else {
      Iteration(Player::P2, Player::P1, tau);
      mu_[1] = (1 - tau) * mu_[1];
    }
    iterations_++;
  }
}

void efg_solve::EGT::Iteration(Player player, Player opponent, double tau) {
  /* compute the expected value associated with each player sequence.
   * Then perform smoothed best response step.*/
  game_->UtilityVector(&average_strategy(opponent), &utility_, player);
  prox_->ProxStep(-1 / mu(player), player, &utility_, &best_response(player));

  /* store the computed response in intermediate_ for creating convex combination with current iterate*/
  CopyContent(best_response(player), &intermediate_, player);
  ConvexCombination(average_strategy(player), &intermediate_, 1-tau, player); // x_hat computed here (Nesterov EGT notation)

  /* compute the expected value associated with each opponent sequence.
   * Then perform smoothed best response to intermediate_. */
  game_->UtilityVector(&intermediate_, &utility_, opponent);
  prox_->ProxStep(-1 / mu(opponent), opponent, &utility_, &best_response(opponent)); // bar{u}_+ computed here (Nesterov EGT notation)

  /* compute the expected value associated with each player sequence.
   * Then perform Bregman projection. */
  game_->UtilityVector(&best_response(opponent), &utility_, player);
  prox_->BregmanProjection(-tau / ((1.0 - tau) / mu(player)), player, &best_response(player), &utility_,
                           &best_response(player));

  /* update current iterates */
  ConvexCombination(best_response(player), &average_strategy(player), tau, player);
  ConvexCombination(best_response(opponent), &average_strategy(opponent), tau, opponent);

  sequences_touched_ += 6 * game_->num_sequences(player);
  sequences_touched_ += 5 * game_->num_sequences(game_->other_player(player));
};

void efg_solve::EGT::WarmStart(std::array<std::vector<double>, 2> strategy_profile) {
  CopyContent(strategy_profile[0], &average_strategy_[0], Player::P1);
  CopyContent(strategy_profile[1], &average_strategy_[1], Player::P2);
}
