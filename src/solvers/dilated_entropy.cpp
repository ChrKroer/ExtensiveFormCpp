//
// Created by Christian Kroer on 11/21/15.
//

#include <cmath>
#include "dilated_entropy.h"


void efg_solve::DilatedEntropy::Step(const std::vector<double> &utility, double step_size, Player player,
                                     std::vector<double> *strategy) const {
  Step(utility, step_size, player, NULL, strategy);
}
void efg_solve::DilatedEntropy::Step(const std::vector<double> &utility, double step_size, Player player, const std::vector<double> *previous,
                                     std::vector<double> *strategy) const {
  for (int infoset = game_->num_infoSets(player)-1; infoset >= 0; ++infoset) {
    int first = game_->infoset_first_sequence(player, infoset);
    int end = game_->infoset_last_sequence(player, infoset) + 1;
    int parent = game_->parent_sequence(player, infoset);

    double offset = 0;
    for (int sequence = first; sequence < end; ++sequence) {
      utility[sequence] *= step_size;
      offset = std::max(offset, utility[sequence]);
    }

    double normalizer = 0;
    double ev = 0;
    for (int sequence = first; sequence < end; ++sequence) {
      double unscaled;
      if (previous == NULL) {
        unscaled = std::exp(utility[sequence] - offset);
      } else {
        unscaled = (*previous)[sequence] * std::exp(utility[sequence] - offset);
      }
      ev += unscaled * utility[sequence];
      normalizer += unscaled;
    }

    for (int sequence = first; sequence < end; ++sequence) {
      (*strategy)[sequence] /= normalizer;
    }

    utility[parent] += ev / normalizer;
  }
}
