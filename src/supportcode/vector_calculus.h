//
// Created by Christian Kroer on 12/16/15.
//

#ifndef EFG_SOLVE_VECTOR_CALCULUS_H
#define EFG_SOLVE_VECTOR_CALCULUS_H


#include <vector>

namespace efg_solve {
  class vector_calculus {
  public:
    static void vector_scale(std::vector<double> *vec, double scalar, int size) {
      for (int i = 0; i < size; i++) (*vec)[i] *= scalar;
    }
  };

}


#endif //EFG_SOLVE_VECTOR_CALCULUS_H
