//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_COIN_H
#define EFG_SOLVERS_COIN_H


#include <vector>
#include <string>
#include "game.h"

namespace efg_solve {

  class Coin : public Game {
  public:
    int num_sequences(Player player) const;

    int num_infosets(Player player) const;

    int infoset_first_sequence(Player player, int infoset) const;

    int infoset_last_sequence(Player player, int infoset) const;

    int infoset_parent_sequence(Player player, int infoset) const;


    int UtilityVector(const std::vector<double> &strategy, std::vector<double> *utility, Player player) const;
    int UtilityVector(const std::vector<double> &strategy, std::vector<double> *utility, Player player, double threshold) const;

    double game_value() { return 0.375; } // hard-coded value of game for debugging ease
  };

}

#endif //EFG_SOLVERS_COIN_H
