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
  int num_sequences(int player) const;
  int num_infosets(int player) const;

  int infoset_first_action(int player, int infoset) const;
  int infoset_last_action(int player, int infoset) const;


  void UtilityVector(double *relization_plan, double *utility_vector, int player) const;

private:
  std::string history;
};


#endif //EFG_SOLVERS_COIN_H
