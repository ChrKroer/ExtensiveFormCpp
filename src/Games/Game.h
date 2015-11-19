//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_GAME_H
#define EFG_SOLVERS_GAME_H


class Game {
public:
  virtual int num_actions(int player) = 0;

  virtual int num_infoSets(int player) = 0;

  virtual int num_actions(int player, int infoSet) = 0;

  virtual bool is_leaf(int history[]) = 0;
};


#endif //EFG_SOLVERS_GAME_H
