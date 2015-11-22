//
// Created by Christian Kroer on 11/20/15.
//

#ifndef EFG_SOLVE_PROX_H
#define EFG_SOLVE_PROX_H


#include <vector>
#include "../games/game.h"

namespace efg_solve {

  class Prox {
  public:
    virtual void Step(const std::vector<double> &utility, double step_size, Player player, std::vector<double> *strategy) const = 0;
    virtual void Step(const std::vector<double> &utility, double step_size, Player player, const std::vector<double> *previous, std::vector<double> *strategy) const = 0;

    void set_game(Game *game) { game_ = game; }

    void set_weights(std::vector<int> weights) { weights_ = weights; }

  private:
    // since games can be quite large, we do not want to keep a copy around.
    Game *game_;
    std::vector<int> weights_;
  };

}

#endif //EFG_SOLVE_PROX_H
