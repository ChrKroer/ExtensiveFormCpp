//
// Created by Christian Kroer on 12/12/15.
//
#include "game.h"

double efg_solve::Game::BestResponseValue(Player player, std::vector<double> *utility) {
  for (int infoset = num_infosets(player)-1; infoset >= 0; --infoset) {
    int first = infoset_first_sequence(player, infoset);
    int last = infoset_last_sequence(player, infoset);
    int parent = parent_sequence(player, infoset);

    double max_utility = std::numeric_limits<double>::lowest();
    for (int i = first; i <= last; ++i) {
      if ((*utility)[i] > max_utility) max_utility = (*utility)[i];
    }
    (*utility)[parent] += max_utility;
  }
  return (*utility)[0];
}
