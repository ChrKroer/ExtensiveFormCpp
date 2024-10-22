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
    typedef std::shared_ptr<Prox> SPtr;

    explicit Prox(Game::SPtr game) {game_ = std::move(game); }

    virtual void ProxStep(double stepsize, Player player, std::vector<double> *utility, std::vector<double> *strategy) const = 0;
    // previous is never modified. However, it is made a pointer to allow passing NULL in.
    // Passing in NULL makes it behave like a regular ProxStep.
    virtual void BregmanProjection(double stepsize, Player player, const std::vector<double> *previous,
                                   std::vector<double> *utility, std::vector<double> *strategy) const = 0;

    void set_game(std::shared_ptr<Game> game) { game_ = game; }

    void set_weights(std::vector<int> weights) { weights_ = weights; }

  private:
    std::vector<int> weights_;
  protected:
    // since games can be quite large, we do not want to keep a copy around.
    Game::SPtr game_;
  };

}  // namespace efg_solve

#endif //EFG_SOLVE_PROX_H
