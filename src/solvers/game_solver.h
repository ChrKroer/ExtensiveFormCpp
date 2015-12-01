//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_GAMESOLVER_H
#define EFG_SOLVERS_GAMESOLVER_H


#include "../Games/game.h"

namespace efg_solve {

  class GameSolver {
  public:
    GameSolver(Game *game_);

    virtual ~GameSolver();

    virtual void solve() = 0;

    std::vector<double> &average_strategy(Player player) {if (player == Player::P1) return average_strategy_[0]; else return average_strategy_[1];}
    //const std::vector<double> &current_strategy(Player player) {if (player == Player::P1) return current_strategy_[0]; else return current_strategy_[1];}

  protected:
    int player_index(Player player) {if (player == Player::P1) return 0; else return 1;}

    /* sets v2 = (1-stepsize)*v2 + stepsize*v1 */
    void ConvexCombination(std::vector<double> &v1, std::vector<double> *v2, double stepsize, Player player);


    // member variables
    Game *game_;
    unsigned int iterations_ = 0;

    std::vector<double> average_strategy_[2];
    std::vector<double> current_strategy_[2];
  };

}

#endif //EFG_SOLVERS_GAMESOLVER_H
