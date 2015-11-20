//
// Created by Christian Kroer on 11/18/15.
//

#include "egt.h"

EGT::EGT(const Game &game, EGT::Prox prox) : GameSolver(game), prox(prox) {

}

EGT::~EGT() {

}

void EGT::UpdateInfoSet(int infoset) {
  if (prox == EGT::Prox::kEntropy) {

  } else if (prox == EGT::Prox::kEuclidean) {

  } else {

  }
}
