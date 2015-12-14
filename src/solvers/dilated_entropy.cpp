//
// Created by Christian Kroer on 11/21/15.
//

#include <cmath>
#include "dilated_entropy.h"
#include "../games/coin.h"

efg_solve::DilatedEntropy::DilatedEntropy(Game *game) : Prox(game) {

}

void efg_solve::DilatedEntropy::ProxStep(double stepsize, Player player, std::vector<double> *utility,
                                         std::vector<double> *strategy) const {
  BregmanProjection(stepsize, player, NULL, utility, strategy);
}
void efg_solve::DilatedEntropy::BregmanProjection(double stepsize, Player player, const std::vector<double> *previous,
                                                  std::vector<double> *utility,
                                                  std::vector<double> *strategy) const {
  (*strategy)[0] = 1;
  for (int infoset = game_->num_infosets(player)-1; infoset >= 0; --infoset) {
    int first = game_->infoset_first_sequence(player, infoset);
    int last = game_->infoset_last_sequence(player, infoset);
    int parent = game_->parent_sequence(player, infoset);

    double offset = 0;
    for (int sequence = first; sequence <= last; ++sequence) {
      (*utility)[sequence] *= stepsize;
      offset = std::max(offset, (*utility)[sequence]);
    }

    double normalizer = 0;
    double ev = 0;
    for (int sequence = first; sequence <= last; ++sequence) {
      if (previous == NULL) { // prox operator
        (*strategy)[sequence] = std::exp((*utility)[sequence] - offset);
      } else { // Bregman projection
        (*strategy)[sequence] = (*previous)[sequence] * std::exp((*utility)[sequence] - offset);
      }
      ev += (*strategy)[sequence] * (*utility)[sequence];
      normalizer += (*strategy)[sequence];
    }

    for (int sequence = first; sequence <= last; ++sequence) {
      (*strategy)[sequence] /= normalizer;
    }

    (*utility)[parent] += ev / normalizer;
  }
}

