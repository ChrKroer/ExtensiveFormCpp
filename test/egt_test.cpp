//
// Created by Christian Kroer on 12/12/15.
//

#include <gtest/gtest.h>
#include "../src/solvers/dilated_entropy.h"
#include "../src/solvers/egt.h"
#include "../src/config.h"
#include "../src/games/game_reader.h"

namespace efg_solve {
class EGTTest : public ::testing::Test {
 public:
  virtual void SetUp() {
    coin = new GameZerosumPackage(config::coin_path);
    coin_prox = new DilatedEntropy(coin);
    coin_egt = new EGT(coin, coin_prox);

    kuhn = new GameZerosumPackage(config::kuhn_path);
    kuhn_prox = new DilatedEntropy(kuhn);
    kuhn_egt = new EGT(kuhn, kuhn_prox);

    prsl = new GameZerosumPackage(config::prsl_path);
    prsl_prox = new DilatedEntropy(prsl);
    prsl_egt = new EGT(prsl, prsl_prox);

    leduc = new GameZerosumPackage(config::leduc_path);
    leduc_prox = new DilatedEntropy(leduc);
    leduc_egt = new EGT(leduc, leduc_prox);

    int max_sequences = std::max( { coin->num_sequences(Player::P1), coin->num_sequences(Player::P2),
                                    kuhn->num_sequences(Player::P1), kuhn->num_sequences(Player::P2),
                                    prsl->num_sequences(Player::P1), prsl->num_sequences(Player::P2),
                                    leduc->num_sequences(Player::P1), leduc->num_sequences(Player::P2)
                                  });
    strategy[0].resize((unsigned long) max_sequences, 0);
    strategy[1].resize((unsigned long) max_sequences, 0);

    utility.resize((unsigned long) max_sequences, 0);
    previous.resize((unsigned long) max_sequences, 0);
  }

  virtual void TearDown() {
    delete coin_egt;
    delete coin_prox;
    delete coin;
  }

  GameZerosumPackage *coin;
  DilatedEntropy *coin_prox;
  EGT *coin_egt;

  GameZerosumPackage *kuhn;
  DilatedEntropy *kuhn_prox;
  EGT *kuhn_egt;

  GameZerosumPackage *prsl;
  DilatedEntropy *prsl_prox;
  EGT *prsl_egt;

  GameZerosumPackage *leduc;
  DilatedEntropy *leduc_prox;
  EGT *leduc_egt;

  std::array<std::vector<double>, 2> strategy;
  std::vector<double> utility;
  std::vector<double> previous;
};

TEST_F(EGTTest, egt_coin_excessive_gap_positive) {
  delete coin_egt;
  config::mu = 10;
  config::gamma = 0.1;
  coin_egt = new EGT(coin, coin_prox);
  EXPECT_GT(coin_egt->excessive_gap(), 0);
}

TEST_F(EGTTest, egt_coin_gap_positive) {
  EXPECT_GT(coin_egt->gap(), 0);
  coin_egt->Run(1);
  double gap = coin_egt->gap();
  EXPECT_GT(gap, 0);
}

TEST_F(EGTTest, egt_coin_improvent_in_bound) {
  config::mu = 1;
  config::gamma = 1;
  coin_egt->Run(1);
  double gap1 = coin_egt->gap();
  coin_egt->Run(1);
  double gap2 = coin_egt->gap();
  EXPECT_LT(gap2, gap1);
}

TEST_F(EGTTest, egt_coin_game_value) {
  coin_egt->Run(1000);
  EXPECT_NEAR(efg_solve::config::coin_game_value, coin->GameValue(&coin_egt->strategy_profile(), &utility), 0.1);
}
TEST_F(EGTTest, egt_kuhn_game_value) {
  kuhn_egt->Run(100000);
  EXPECT_NEAR(efg_solve::config::kuhn_game_value, kuhn->GameValue(&kuhn_egt->strategy_profile(), &utility), 0.1);
}
TEST_F(EGTTest, egt_prsl_game_value) {
  prsl_egt->Run(1000);
  EXPECT_NEAR(efg_solve::config::prsl_game_value, prsl->GameValue(&prsl_egt->strategy_profile(), &utility), 0.1);
}
TEST_F(EGTTest, egt_leduc_game_value) {
  leduc_egt->Run(1000);
  EXPECT_NEAR(efg_solve::config::leduc_game_value, leduc->GameValue(&leduc_egt->strategy_profile(), &utility), 0.1);
}
}
