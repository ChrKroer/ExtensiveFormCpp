//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_GAME_H
#define EFG_SOLVERS_GAME_H


#include <vector>

namespace efg_solve {

  enum class Player {
    P1 = 0, P2 = 1
  };

  class Game {
  public:
    const int NUM_PLAYERS = 2;

    // return the total number of sequences available to the player.
    virtual int num_sequences(Player player) const = 0;

    virtual int num_infoSets(Player player) const = 0;

    virtual int infoset_first_sequence(Player player, int infoset) const = 0;

    virtual int infoset_last_sequence(Player player, int infoset) const = 0;

    virtual int parent_sequence(Player player, int infoset) const = 0;

    /**
     * Fills in the utility vector of player using the strategy of the opposing player.
     * Expected values are computed such that each player wishes to maximize the returned values.
     */
    virtual void UtilityVector(const std::vector<double> &strategy, std::vector<double> *utility, Player player) const = 0;

    Player other_player(Player player) { if (player == Player::P1) return Player::P2; else return Player::P1; }
  };

}

#endif //EFG_SOLVERS_GAME_H
