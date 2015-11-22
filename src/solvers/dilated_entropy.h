//
// Created by Christian Kroer on 11/21/15.
//

#ifndef EFG_SOLVE_DILATED_ENTROPY_H
#define EFG_SOLVE_DILATED_ENTROPY_H


#include "prox.h"

namespace efg_solve {
  class DilatedEntropy : public Prox {

  public:
    virtual void Step(const std::vector<double> &utility, double step_size, Player player, std::vector<double> *strategy) const;
    virtual void Step(const std::vector<double> &utility, double step_size, Player player, const std::vector<double> *previous, std::vector<double> *strategy) const;
  };

}
#endif //EFG_SOLVE_DILATED_ENTROPY_H
