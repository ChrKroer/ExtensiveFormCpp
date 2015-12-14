//
// Created by Christian Kroer on 12/12/15.
//

#include <iosfwd>

#ifndef EFG_SOLVE_CONFIG_H
#define EFG_SOLVE_CONFIG_H

namespace efg_solve {
  namespace config {
    extern double mu;
    extern double gamma;
    extern double coin_game_value;

    void load_config_file();
  }
}

#endif //EFG_SOLVE_CONFIG_H
