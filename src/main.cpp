#include <iostream>
#include <tclap/CmdLine.h>
#include "games/game_reader.h"
#include "solvers/egt.h"
#include "solvers/dilated_entropy.h"

namespace efg_solve {
int run(std::string gamepath, GameName gamename, double mu, double gamma, int runs, int iterations_per_run) {
  GameTree::SPtr game = GameZerosumPackage::CreateGameFromFile(gamepath, gamename);
  DilatedEntropy::SPtr game_prox = DilatedEntropy::SPtr(new DilatedEntropy(game));
  EGT::UPtr egt = EGT::UPtr(new EGT(game, game_prox));

  for (int run = 0; run < runs; ++run) {
    egt->Run(iterations_per_run);
    int sequence_touches = egt->sequences_touched();
    double ev = game->GameValue(&egt->strategy_profile());
    double gap = egt->gap();
    double excessive_gap = egt->excessive_gap();

    printf("%d %.4f %.4f %.4f", sequence_touches, ev, gap, excessive_gap);
  }
}
}

int main(int argc, char* argv[]) {
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

    // Parse the argv array.
    cmd.parse(argc, (const char *const *) argv);

    // Get the value parsed by each arg.
    std::string filename = fileArg.getValue();
    std::string algorithm = algoArg.getValue();
    int runs = runsArg.getValue();
    int iterations = itersArg.getValue();
    double mu = muArg.getValue();
    double gamma = gammaArg.getValue();


    efg_solve::run(filename, efg_solve::GameName::LEDUC, mu, gamma, runs, iterations);

  } catch (TCLAP::ArgException &e)  // catch any exceptions
  { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }
}

