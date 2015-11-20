//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_COIN_H
#define EFG_SOLVERS_COIN_H


#include <vector>
#include <string>
#include "game.h"

class Coin : public Game {
public:
  int num_sequences(int player);
  int num_infosets(int player);

  void UtilityVector(double relization_plan[], double utility_vector[], int player);

private:
  std::string history;
};


#endif //EFG_SOLVERS_COIN_H
