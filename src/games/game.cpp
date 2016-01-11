//
// Created by Christian Kroer on 12/12/15.
//
#include <array>
#include "game.h"

double efg_solve::Game::BestResponseValue(Player player, std::vector<double> *utility) const {
  for (int infoset = num_infosets(player)-1; infoset >= 0; --infoset) {
    int first = infoset_first_sequence(player, infoset);
    int last = infoset_last_sequence(player, infoset);
    int parent = infoset_parent_sequence(player, infoset);

    double val = std::numeric_limits<double>::lowest();
    for (int i = first; i <= last; ++i) {
      if ((*utility)[i] > val) val = (*utility)[i];
    }
    (*utility)[parent] += val;
  }
  return (*utility)[0];
}

double efg_solve::Game::GameValue(std::array<std::vector<double>, 2> *strategy_profile) const {
  int max_seq = std::max(num_sequences(Player::P1), num_sequences(Player::P2));
  std::vector<double> utility((unsigned long) max_seq, 0);
  return GameValue(strategy_profile, &utility);
}

double efg_solve::Game::GameValue(std::array<std::vector<double>, 2> *strategy_profile, std::vector<double> *utility) const {
  return GameValue(strategy_profile, utility, Player::P1);
}

double efg_solve::Game::GameValue(std::array<std::vector<double>, 2> *strategy_profile, std::vector<double> *utility, Player player) const {
  UtilityVector(&(*strategy_profile)[player_id(other_player(player))], utility, player);
  for (int infoset = num_infosets(player)-1; infoset >= 0; --infoset) {
    int first = infoset_first_sequence(player, infoset);
    int last = infoset_last_sequence(player, infoset);
    int parent = infoset_parent_sequence(player, infoset);

    for (int i = first; i <= last; ++i) {
      double sequence_probability = (*strategy_profile)[player_id(player)][i];
      double expected_payoff = (*utility)[i];
      (*utility)[parent] += sequence_probability * expected_payoff;
    }
  }
  if (player == Player::P1) {
    return (*utility)[0];
  } else {
    return -(*utility)[0];
  }
}


void efg_solve::Game::InitUniform(std::vector<double> *strategy, Player player) const {
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

void efg_solve::Game::ToSequenceForm(std::vector<double> *strategy, Player player) const {
  for (int infoset = 0; infoset < num_infosets(player); ++infoset) {
    int first = infoset_first_sequence(player, infoset);
    int last = infoset_last_sequence(player, infoset);
    int parent = infoset_parent_sequence(player, infoset);

    for (int i = first; i <= last; ++i) {
      (*strategy)[i] *= (*strategy)[parent];
    }
  }
}

void efg_solve::Game::ToBehavioralStrategy(std::vector<double> *strategy, Player player) const {
  for (int infoset = num_infosets(player)-1; infoset >= 0; --infoset) {
    int first = infoset_first_sequence(player, infoset);
    int last = infoset_last_sequence(player, infoset);
    int parent = infoset_parent_sequence(player, infoset);

    for (int i = first; i <= last; ++i) {
      if ((*strategy)[parent] > 0) {
        (*strategy)[i] /= (*strategy)[parent];
      } else {
        (*strategy)[i] = 0;
      }
    }
  }
}
