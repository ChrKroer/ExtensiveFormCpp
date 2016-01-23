//
// Created by Christian Kroer on 11/18/15.
//

#include <cmath>
#include "egt.h"
#include "../config.h"

efg_solve::EGT::EGT(Game::SPtr game, Prox::SPtr prox) : EGT(game, prox, config::mu, config::gamma, 0) {

}

efg_solve::EGT::EGT(Game::SPtr game, Prox::SPtr prox, double mu, double gamma) : EGT(game, prox, config::mu, config::gamma, 0) {

}

efg_solve::EGT::EGT(Game::SPtr game, Prox::SPtr prox, double mu, double gamma, double threshold) : GameSolver(std::move(game)), prox_(std::move(prox)), gamma_(gamma), threshold_(threshold) {
  mu_[0] = 1.0/gamma;
  mu_[1] = mu;
  Init();
}

efg_solve::EGT::~EGT() {

}

double efg_solve::EGT::excessive_gap() {
  double x,y;
  return excessive_gap(x, y);
}

double efg_solve::EGT::excessive_gap(double &ev_x, double &ev_y) {
  game_->UtilityVector(average_strategy(Player::P1), &utility_, Player::P2);
  prox_->ProxStep(-1 / mu(Player::P2), Player::P2, &utility_, &best_response(Player::P2));
  // ProxStep converts to a minimization problem for P2, so we need to negate the return value.
  ev_x = -utility_[0] / mu(Player::P2);

  game_->UtilityVector(average_strategy(Player::P2), &utility_, Player::P1);
  prox_->ProxStep(-1 / mu(Player::P1), Player::P1, &utility_, &best_response(Player::P1));
  // ProxStep converts to a minimization problem for P1, which is what we want.
  ev_y = utility_[0] / mu(Player::P1);

  //printf("(%0.08lf, %0.08lf)\n", smooth_br_val_phi, smooth_br_val_f);
  return ev_y - ev_x;
}

double efg_solve::EGT::gap() {
  double x,y;
  return gap(x, y);
};

double efg_solve::EGT::gap(double &ev_x, double &ev_y) {
  game_->UtilityVector(average_strategy(Player::P1), &utility_, Player::P2);
  ev_x = game_->BestResponseValue(Player::P2, &utility_);
  game_->UtilityVector(average_strategy(Player::P2), &utility_, Player::P1);
  ev_y = -game_->BestResponseValue(Player::P1, &utility_);
  return ev_x - ev_y;
};

void efg_solve::EGT::Init() {
  unsigned int max_sequences = (unsigned int) std::max(game_->num_sequences(Player::P1), game_->num_sequences(Player::P2));
  best_response_[0].resize((unsigned long) game_->num_sequences(Player::P1), 0);
  best_response_[1].resize((unsigned long) game_->num_sequences(Player::P2), 0);
  intermediate_.resize(max_sequences, 0);
  utility_.resize(max_sequences, 0);

  game_->InitUniform(&average_strategy(Player::P1), Player::P1);
  game_->InitUniform(&average_strategy(Player::P2), Player::P2);
  double ev_x, ev_y;
//  printf("\ninitial gap           = %0.08lf (%0.08lf, %0.08lf)\n", gap(ev_x, ev_y), ev_x, ev_y);
//  printf("initial excessive gap = %0.08lf (%0.08lf, %0.08lf)\n\n", excessive_gap(ev_x, ev_y), ev_y, ev_x);
  // prox step P2
  game_->UtilityVector(average_strategy(Player::P1), &utility_, Player::P2);
  prox_->ProxStep(-1 / mu(Player::P2), Player::P2, &utility_, &average_strategy(Player::P2));

  // Bregman projection P1
  game_->UtilityVector(average_strategy(Player::P2), &utility_, Player::P1);
  prox_->BregmanProjection(-gamma_, Player::P1, &average_strategy(Player::P1), &utility_,
                           &average_strategy(Player::P1));

  printf("\ninitial gap           = %0.08lf (%0.08lf, %0.08lf)\n", gap(ev_x, ev_y), ev_x, ev_y);
  printf("initial excessive gap = %0.08lf (%0.08lf, %0.08lf)\n\n", excessive_gap(ev_x, ev_y), ev_y, ev_x);
};

void efg_solve::EGT::Run(int num_iterations) {
  for (int iterate = 0; iterate < num_iterations; ++iterate) {
    double tau = 2.0 / (iterations_ + 3);
    double old_gap = game_->MaxRegret(strategy_profile());

    if (mu(Player::P2) > mu(Player::P1)) {
      Iteration(Player::P1, Player::P2, tau);
      mu_[1] = (1 - tau) * mu_[1];
    } else {
      Iteration(Player::P2, Player::P1, tau);
      mu_[0] = (1 - tau) * mu_[0];
    }
    iterations_++;
    double new_gap = game_->MaxRegret(strategy_profile());
  }
}

void efg_solve::EGT::Iteration(Player player, Player opponent, double tau) {
  double ev_x, ev_y;
  double current_threshold = std::min(threshold_ / iterations_, 0.01);

  /* compute the expected value associated with each player sequence.
   * Then compute smoothed best response.*/
  sequences_touched_ += 3 * game_->UtilityVector(average_strategy(opponent), &utility_, player, current_threshold);
  prox_->ProxStep(-1 / mu(player), player, &utility_, &best_response(player));

  /* store the computed response in intermediate_ for creating convex combination with current iterate*/
  CopyContent(best_response(player), &intermediate_, player);
  Combination(1-tau, average_strategy(player), tau, &intermediate_, player); // x_hat computed here (Nesterov EGT notation)

  /* compute the expected value associated with each opponent sequence.
   * Then perform smoothed best response to intermediate_. */
  sequences_touched_ += 3 * game_->UtilityVector(intermediate_, &utility_, opponent, current_threshold);
  prox_->ProxStep(-1 / mu(opponent), opponent, &utility_, &best_response(opponent)); // bar{u}_+ computed here (Nesterov EGT notation)
//  if (player == Player::P1) {
//    std::array<std::vector<double>, 2> temp_profile = {intermediate_, best_response(opponent)};
//    printf("epsilon           = %0.08lf (%0.08lf, %0.08lf)\n", game_->MaxRegret(&temp_profile, ev_x, ev_y), ev_x, ev_y);
//  }
  /* new iterate for opponent */
  Combination(tau, best_response(opponent), 1-tau, &average_strategy(opponent), opponent);

//  if (player == Player::P1) {
//    std::array<std::vector<double>, 2> temp_profile = {intermediate_, average_strategy(opponent)};
//    printf("epsilon           = %0.08lf (%0.08lf, %0.08lf)\n", game_->MaxRegret(&temp_profile, ev_x, ev_y), ev_x, ev_y);
//  }

  /* compute the expected value associated with each player sequence.
   * Then perform Bregman projection. */
  sequences_touched_ += 3 * game_->UtilityVector(best_response(opponent), &utility_, player, current_threshold);
  prox_->BregmanProjection(-tau / ((1.0 - tau) * mu(player)), player, &best_response(player), &utility_, &intermediate_);

//  if (player == Player::P1) {
//    std::array<std::vector<double>, 2> temp_profile = {intermediate_, average_strategy(opponent)};
//    printf("epsilon           = %0.08lf (%0.08lf, %0.08lf)\n", game_->MaxRegret(&temp_profile, ev_x, ev_y), ev_x, ev_y);
//  }

  Combination(tau, intermediate_, 1-tau, &average_strategy(player), player);

  //sequences_touched_ += 6 * game_->num_sequences(player);
  //sequences_touched_ += 5 * game_->num_sequences(game_->other_player(player));
};

void efg_solve::EGT::WarmStart(std::array<std::vector<double>, 2> strategy_profile) {
  CopyContent(strategy_profile[0], &average_strategy_[0], Player::P1);
  CopyContent(strategy_profile[1], &average_strategy_[1], Player::P2);
}
