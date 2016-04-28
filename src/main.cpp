#include <iostream>
#include <tclap/CmdLine.h>
#include "games/game_reader.h"
#include "solvers/egt.h"
#include "solvers/dilated_entropy.h"
#include "config.h"

namespace efg_solve {
void print_progress(EGT::UPtr &egt, GameTree::SPtr &game) {
  int sequence_touches = egt->sequences_touched();
  double ev = game->GameValue(egt->strategy_profile());
  double br_x, br_y, excessive_x, excessive_y;
  double gap = egt->gap(br_x, br_y);
  double regret1, regret2;
  double epsilon = game->MaxRegret(egt->strategy_profile(), regret1, regret2);
  std::array<double, 2> mu = egt->mu();
  double excessive_gap = egt->excessive_gap(excessive_x, excessive_y);

  printf("t=%d, seqs=%d, ev=%.06lf, epsilon=%.06lf (%.06lf, %.06lf), mu=(%.06lf, %.06lf), egc=%.06lf (%.06lf, %.06lf)\n", egt->num_iterations(), sequence_touches, ev, epsilon, regret1, regret2, mu[0], mu[1], excessive_gap, excessive_y, excessive_x);
}

bool stop_running(EGT::UPtr &egt, GameTree::SPtr &game) {
  double regret1, regret2;
  double epsilon = game->MaxRegret(egt->strategy_profile(), regret1, regret2);
  return epsilon < 0.001;
}

void run(std::string gamepath, GameName gamename, double mu, double gamma, int runs, int iterations_per_run, double threshold) {
  GameTree::SPtr game;
  if (gamename == GameName::LEDUC) {
    game = GameZerosumPackage::CreateGameFromFile(gamepath, gamename);
  } else {
    game = GameZerosumPackage::CreateGameFromFile(gamepath);
  }

  DilatedEntropy::SPtr game_prox = DilatedEntropy::SPtr(new DilatedEntropy(game));
  EGT::UPtr egt = EGT::UPtr(new EGT(game, game_prox, mu, gamma, threshold));

  print_progress(egt, game);
  for (int run = 0; run < runs; ++run) {
    egt->Run(iterations_per_run);
    print_progress(egt, game);
  }
}
}

int main(int argc, char* argv[]) {
  efg_solve::run(efg_solve::config::coin_path, efg_solve::GameName::OTHER, 1.5, 2, 95, 100, 0.00);
  //efg_solve::run(efg_solve::config::kuhn_path, efg_solve::GameName::OTHER, 1.5, 2, 10, 1000, 0.00);
  //efg_solve::run(efg_solve::config::leduckj1_path, efg_solve::GameName::LEDUC, 1.5, 2, 20, 1000, 0.00);
  //efg_solve::run(efg_solve::config::leduck2_path, efg_solve::GameName::LEDUC, 1.5, 2, 20, 1000, 0.00);
  //efg_solve::run(efg_solve::config::leduckj2_path, efg_solve::GameName::LEDUC, 1.5, 2, 12, 1000, 0.00);
  //efg_solve::run(efg_solve::config::leduc1raise_path, efg_solve::GameName::LEDUC, 1.5, 2, 20, 2000, 0.00);
  //efg_solve::run(efg_solve::config::leduc_path, efg_solve::GameName::LEDUC, 1.5, 2, 8, 1000, 0.00);
  //efg_solve::run(efg_solve::config::leduc_4betsizes_path, efg_solve::GameName::LEDUC, 3.5, 5, 8, 1000, 0.00);
  return 0;
  try {
    TCLAP::CmdLine cmd("Library for solving extensive-form games.", ' ', "");

    TCLAP::ValueArg<std::string> fileArg("f", "file", "Which game file to use", true, "none", "A path to a game file in zerosum format");
    cmd.add(fileArg);

    TCLAP::ValueArg<std::string> algoArg("a", "algorithm", "Which algorithm to use", false, "egt", "egt.");
    cmd.add(algoArg);

    TCLAP::ValueArg<int> runsArg("r","runs","How many runs to do", false, 2, "positive integer");
    cmd.add(runsArg);

    TCLAP::ValueArg<int> itersArg("i","iterations","How many iterations to do per run", false, 5, "positive integer");
    cmd.add(itersArg);

    TCLAP::ValueArg<double> muArg("m","mu","The initial prox-function weight for player 1", false, 1, "positive float");
    cmd.add(muArg);

    TCLAP::ValueArg<double> gammaArg("g","gamma","The initial prox-function weight for player 2 is the inverse of this", false, 1, "positive float");
    cmd.add(gammaArg);

    TCLAP::ValueArg<double> thresholdArg("t","threshold","The threshold applied for gradient computation", false, 0, "float");
    cmd.add(thresholdArg);

    // Parse the argv array.
    cmd.parse(argc, (const char *const *) argv);

    // Get the value parsed by each arg.
    std::string filename = fileArg.getValue();
    std::string algorithm = algoArg.getValue();
    int runs = runsArg.getValue();
    int iterations = itersArg.getValue();
    double mu = muArg.getValue();
    double gamma = gammaArg.getValue();
    double threshold = thresholdArg.getValue();
    efg_solve::GameName gamename = efg_solve::GameName::OTHER;


    efg_solve::run(filename, efg_solve::GameName::LEDUC, mu, gamma, runs, iterations, threshold);

  } catch (TCLAP::ArgException &e)  // catch any exceptions
  { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }
}

