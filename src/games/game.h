//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_GAME_H
#define EFG_SOLVERS_GAME_H


#include <vector>

class Game {
public:
  // return the total number of sequences available to the player.
  virtual int num_sequences(int player) const = 0;

  virtual int num_infoSets(int player) const = 0;

  virtual int infoset_first_action(int player, int infoset) const = 0;
  virtual int infoset_last_action(int player, int infoset) const = 0;

  virtual void UtilityVector(double realization_plan[], double utility[], int player) const = 0;
};


#endif //EFG_SOLVERS_GAME_H
