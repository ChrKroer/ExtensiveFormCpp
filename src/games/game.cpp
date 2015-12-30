//
// Created by Christian Kroer on 12/12/15.
//
#include <array>
#include "game.h"

double efg_solve::Game::BestResponseValue(Player player, std::vector<double> *utility) {
  for (int infoset = num_infosets(player)-1; infoset >= 0; --infoset) {
    int first = infoset_first_sequence(player, infoset);
    int last = infoset_last_sequence(player, infoset);
    int parent = parent_sequence(player, infoset);

    double val = std::numeric_limits<double>::lowest();
    for (int i = first; i <= last; ++i) {
      if ((*utility)[i] > val) val = (*utility)[i];
    }
    (*utility)[parent] += val;
  }
  return (*utility)[0];
}

double efg_solve::Game::GameValue(const std::array<std::vector<double>, 2> &strategy_profile) {
  int max_seq = std::max(num_sequences(Player::P1), num_sequences(Player::P2));
  std::vector<double> utility(max_seq, 0);
  return GameValue(strategy_profile, &utility);
}

double efg_solve::Game::GameValue(const std::array<std::vector<double>, 2> &strategy_profile, std::vector<double> *utility) {
  UtilityVector(strategy_profile[1], utility, Player::P1);
  for (int infoset = num_infosets(Player::P1)-1; infoset >= 0; --infoset) {
    int first = infoset_first_sequence(Player::P1, infoset);
    int last = infoset_last_sequence(Player::P1, infoset);
    int parent = parent_sequence(Player::P1, infoset);

    for (int i = first; i <= last; ++i) {
      (*utility)[parent] += strategy_profile[0][i] * (*utility)[i];
    }
  }
  return (*utility)[0];
}

void efg_solve::Game::InitUniform(std::vector<double> *strategy, Player player) {
  (*strategy)[0] = 1;
  for (int infoset = num_infosets(player)-1; infoset >= 0; --infoset) {
    int first = infoset_first_sequence(player, infoset);
    int last = infoset_last_sequence(player, infoset);
    double uniform_probability = 1.0 / (last - first + 1);

    for (int i = first; i <= last; ++i) {
      (*strategy)[i] = uniform_probability;
    }
  }
}
