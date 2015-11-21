//
// Created by Christian Kroer on 11/19/15.
//

#ifndef EFG_SOLVE_FULLGAME_H
#define EFG_SOLVE_FULLGAME_H

#include <string>
#include "game.h"

class FullGame : public Game {
public:
  FullGame();
  ~FullGame();
  void ReadFromZerosumFile(std::string path);

  int num_sequences(int player) {return num_sequences_[player];}
  int num_infosets(int player) {return num_info_sets_[player];}

  void UtilityVector(double realization_plan[], double utility[], int player);

private:
  int num_info_sets_[2];
  int num_sequences_[2];

  int nodes_[];


  // game tree
};


#endif //EFG_SOLVE_FULLGAME_H
