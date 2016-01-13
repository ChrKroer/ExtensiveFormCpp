//
// Created by Christian Kroer on 12/12/15.
//

#include <string>
#include "config.h"

namespace efg_solve {
namespace config {
double mu = 5;
double gamma = 0.2;
double coin_game_value = 0.375;
double kuhn_game_value = -0.0555556;
double prsl_game_value = 0;
double leduc_game_value = -0.0856064;

std::string original_game_files = "/Users/ckroer/Documents/research/zerosum/original_games/";
std::string coin_path = original_game_files + "coin.txt";
std::string kuhn_path = original_game_files + "kuhn.txt";
std::string prsl_path = original_game_files + "prsl.txt";
std::string leduc_path = original_game_files + "leduc.txt";

std::string leduc_equilibrium_p1 = "/Users/ckroer/Documents/research/zerosum/equilibrium_strategies/leducx.txt";
std::string leduc_equilibrium_p2 = "/Users/ckroer/Documents/research/zerosum/equilibrium_strategies/leducy.txt";
}
}