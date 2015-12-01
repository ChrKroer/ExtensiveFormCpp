//
// Created by Christian Kroer on 11/18/15.
//

#include "egt.h"

efg_solve::EGT::EGT(Game *game, Prox *prox) : GameSolver(game), prox_(prox) {
  unsigned int max_sequences = (unsigned int) std::max(game->num_sequences(Player::P1), game->num_sequences(Player::P2));
  best_response_[0].resize((unsigned long) game->num_sequences(Player::P1), 0);
  best_response_[1].resize((unsigned long) game->num_sequences(Player::P2), 0);
  intermediate_.resize(max_sequences, 0);
  utility_.resize(max_sequences, 0);
}

efg_solve::EGT::~EGT() {

}

void efg_solve::EGT::Iterate(int num_iterations) {
  for (int iterate = 0; iterate < num_iterations; ++iterate) {
    double tau = 2.0 / (iterations_ + 3);

    if (mu_[1] > mu_[0]) {
      /* compute the expected value associated with each P1 sequence.
       * Then perform prox step.*/
      game_->UtilityVector(average_strategy(Player::P2), &utility_, Player::P1);
      prox_->Step(utility_, -1/mu_[0], Player::P1, &best_response_[0]);

      /* store the computed response in intermediate_ for creating convex combination with current iterate*/
      ConvexCombination(best_response_[0], &intermediate_, 1, Player::P1);
      ConvexCombination(average_strategy(Player::P1), &intermediate_, 1-tau, Player::P1);

      /* compute the expected value associated with each P2 sequence.
       * Then perform smoothed best response to intermediate_. */
      game_->UtilityVector(intermediate_, &utility_, Player::P2);
      prox_->Step(utility_, -1/mu_[1], Player::P2, &best_response_[1]);

      /* compute the expected value associated with each P1 sequence.
       * Then perform prox step. */
      game_->UtilityVector(best_response_[1], &utility_, Player::P1);
      prox_->Step(utility_, -tau/((1.0-tau)/mu_[0]), Player::P1, best_response_[0], &best_response_[0]);

      /* update current iterates */
      ConvexCombination(best_response_[0], &average_strategy(Player::P1), tau, Player::P1);
      ConvexCombination(best_response_[1], &average_strategy(Player::P2), tau, Player::P2);

      mu_[1] = (1.0 - tau) * mu_[1];
    } else {

      mu_[0] = (1.0 - tau) * mu_[0];
    }
    iterations_++;
  }
}

