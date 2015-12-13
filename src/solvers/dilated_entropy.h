//
// Created by Christian Kroer on 11/21/15.
//

#ifndef EFG_SOLVE_DILATED_ENTROPY_H
#define EFG_SOLVE_DILATED_ENTROPY_H


#include "prox.h"
#include "../games/coin.h"

namespace efg_solve {
  class DilatedEntropy : public Prox {

  public:
    DilatedEntropy(Game *game);

    virtual void ProxStep(double stepsize, Player player, std::vector<double> *utility, std::vector<double> *strategy) const;
    virtual void BregmanProjection(double stepsize, Player player, const std::vector<double> *previous,
                                   std::vector<double> *utility, std::vector<double> *strategy) const;
  };

}
#endif //EFG_SOLVE_DILATED_ENTROPY_H
