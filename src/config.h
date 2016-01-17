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
extern double kuhn_game_value;
extern double prsl_game_value;
extern double leduc_game_value;

extern std::string coin_path;
extern std::string kuhn_path;
extern std::string prsl_path;
extern std::string leduc_path;

extern std::string leduc_equilibrium_p1;
extern std::string leduc_equilibrium_p2;

extern std::string data_folder;
extern std::string strategies_folder;
extern std::string cfrx_strategy_path;
extern std::string cfry_strategy_path;
}
}

#endif //EFG_SOLVE_CONFIG_H
