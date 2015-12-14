//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_GAMESOLVER_H
#define EFG_SOLVERS_GAMESOLVER_H


#include <array>
#include "../Games/game.h"

namespace efg_solve {

  typedef std::vector<double> strategy_profile[2];

  class GameSolver {
  public:
    GameSolver(Game *game_);

    virtual ~GameSolver();

    virtual void Run(int num_iterations) = 0;

    //auto strategy_profile() -> const std::vector<double> (&)[2] {return average_strategy_; }
    std::array<std::vector<double>, 2> strategy_profile() { return average_strategy_; }

    std::vector<double> &average_strategy(Player player) {
      if (player == Player::P1) return average_strategy_[0]; else return average_strategy_[1];
    }

  protected:
    int player_index(Player player) {if (player == Player::P1) return 0; else return 1;}

    /* sets v2 = (1-stepsize)*v2 + stepsize*v1 */
    void ConvexCombination(const std::vector<double> &v1, std::vector<double> *v2, double stepsize, Player player);
    /* Copies the contents of v1 into v2 for the number of sequences belonging to player */
    void CopyContent(const std::vector<double> &v1, std::vector<double> *v2, Player player);


    // member variables
    Game *game_;
    unsigned int iterations_ = 0;

    std::array<std::vector<double>, 2> average_strategy_;
  };

}

#endif //EFG_SOLVERS_GAMESOLVER_H
