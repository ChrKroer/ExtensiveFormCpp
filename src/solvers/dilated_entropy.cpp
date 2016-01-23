//
// Created by Christian Kroer on 11/21/15.
//

#include <cmath>
#include "dilated_entropy.h"
#include "../supportcode/vector_calculus.h"

efg_solve::DilatedEntropy::DilatedEntropy(Game::SPtr game) : Prox(std::move(game)) {

}

void efg_solve::DilatedEntropy::ProxStep(double stepsize, Player player, std::vector<double> *utility,
                                         std::vector<double> *strategy) const {
  BregmanProjection(stepsize, player, NULL, utility, strategy);
}
void efg_solve::DilatedEntropy::BregmanProjection(double stepsize, Player player, const std::vector<double> *previous,
                                                  std::vector<double> *utility,
                                                  std::vector<double> *strategy) const {
  //vector_calculus::vector_scale(utility, -stepsize, game_->num_sequences(player));
  for (int infoset = game_->num_infosets(player)-1; infoset >= 0; --infoset) {
    int first = game_->infoset_first_sequence(player, infoset);
    int last = game_->infoset_last_sequence(player, infoset);
    int parent = game_->infoset_parent_sequence(player, infoset);

    double offset = -stepsize * (*utility)[first];
    for (int sequence = first+1; sequence <= last; ++sequence) {
      offset = std::max(offset, -stepsize * (*utility)[sequence]);
    }

    double normalizer = 0;
    double ev = 0;
    for (int sequence = first; sequence <= last; ++sequence) {
      if (previous == NULL) { // prox operator
        (*strategy)[sequence] = std::exp(-stepsize*(*utility)[sequence] - offset);
      } else { // Bregman projection
        (*strategy)[sequence] = (*previous)[sequence] * std::exp(-stepsize * (*utility)[sequence] - offset);
      }
      ev += (*strategy)[sequence] * (*utility)[sequence];
      normalizer += (*strategy)[sequence];
    }

    normalizer = 1.0 / normalizer;
    for (int sequence = first; sequence <= last; ++sequence) {
      (*strategy)[sequence] *= normalizer;
    }

    (*utility)[parent] += ev * normalizer;
  }
  (*strategy)[0] = 1;
}

