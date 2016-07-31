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

std::string zerosum_folder = "/Users/ckroer/Dropbox/research/equilibrium-"
    "finding/zerosum/";

std::string original_game_files = zerosum_folder + "original_games/";
std::string coin_path = original_game_files + "coin.txt";
std::string kuhn_path = original_game_files + "kuhn.txt";
std::string prsl_path = original_game_files + "prsl.txt";


std::string data_folder = "/Users/ckroer/Documents/research/equilibrium_"
    "computation/data/";
std::string strategies_folder = data_folder + "/strategies/";
std::string cfrx_strategy_path = strategies_folder + "leduc_cfrx.txt";
std::string cfry_strategy_path = strategies_folder + "leduc_cfry.txt";


double leduc_game_value = -0.0856064;
std::string leduc_path = original_game_files + "leduc.txt";
std::string leduc_equilibrium_p1 = zerosum_folder +
    "equilibrium_strategies/leducx.txt";
std::string leduc_equilibrium_p2 = zerosum_folder +
    "equilibrium_strategies/leducy.txt";


double leduc1raise_game_value = -0.0781063;
std::string leduc1raise_path = original_game_files + "leduc1raise.txt";
std::string leduc1raise_equilibrium_p1 = zerosum_folder +
    "equilibrium_strategies/leduc1raisex.txt";
std::string leduc1raise_equilibrium_p2 = zerosum_folder +
    "equilibrium_strategies/leduc1raisey.txt";


double leduckj1_game_value = 0;
std::string leduckj1_path = original_game_files + "leduckj1.txt";
std::string leduckj1_equilibrium_p1 = zerosum_folder +
    "equilibrium_strategies/leduckj1x.txt";
std::string leduckj1_equilibrium_p2 = zerosum_folder +
    "equilibrium_strategies/leduckj1y.txt";


double leduck2_game_value = 0;
std::string leduck2_path = original_game_files + "leduck2.txt";
std::string leduck2_equilibrium_p1 = zerosum_folder +
    "equilibrium_strategies/leduck2x.txt";
std::string leduck2_equilibrium_p2 = zerosum_folder +
    "equilibrium_strategies/leduck2y.txt";


double leduckj2_game_value = 0;
std::string leduckj2_path = original_game_files + "leduckj2.txt";
std::string leduckj2_equilibrium_p1 = zerosum_folder +
    "equilibrium_strategies/leduckj2x.txt";
std::string leduckj2_equilibrium_p2 = zerosum_folder +
    "equilibrium_strategies/leduckj2y.txt";


double leduc_4betsizes_game_value = -0.0852061;
std::string leduc_4betsizes_path = original_game_files + "leduc_4betsizes.txt";
std::string leduc_4betsizes_equilibrium_p1 = zerosum_folder +
    "equilibrium_strategies/leduc_4betsizesx.txt";
std::string leduc_4betsizes_equilibrium_p2 = zerosum_folder +
    "equilibrium_strategies/leduc_4betsizesy.txt";
}
}