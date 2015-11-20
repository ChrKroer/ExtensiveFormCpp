//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_GAME_H
#define EFG_SOLVERS_GAME_H


#include <vector>

class Game {
public:
  // return the total number of sequences available to the player.
  virtual int num_sequences(int player) = 0;

  virtual int num_infoSets(int player) = 0;

  virtual void UtilityVector(double realization_plan[], double utility[], int player) = 0;
};


#endif //EFG_SOLVERS_GAME_H
