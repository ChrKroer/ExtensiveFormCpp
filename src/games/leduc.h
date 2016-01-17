//
// Created by Christian Kroer on 12/17/15.
//

#ifndef EFG_SOLVE_LEDUC_H
#define EFG_SOLVE_LEDUC_H


#include <string>
#include "game.h"

namespace efg_solve {
  class Leduc : public Game {

  public:
    int num_sequences(Player player) const;

    int num_infosets(Player player) const;

    int infoset_first_sequence(Player player, int infoset) const;

    int infoset_last_sequence(Player player, int infoset) const;

    int parent_sequence(Player player, int infoset) const;


    void UtilityVector(std::vector<double> *strategy, std::vector<double> *utility, Player player) const;

    double game_value() { return -0.0856064; } // hard-coded value of game for debugging ease

  };
}


#endif //EFG_SOLVE_LEDUC_H
