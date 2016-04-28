//
// Created by Christian Kroer on 12/12/15.
//

#include <memory>
#include <gtest/gtest.h>
#include "../src/solvers/dilated_entropy.h"
#include "../src/solvers/egt.h"
#include "../src/config.h"
#include "../src/games/game_reader.h"
#include "../src/games/strategy_io.h"

namespace efg_solve {
class EGTTest : public ::testing::Test {
 public:
  virtual void SetUp() {
    coin = GameZerosumPackage::CreateGameFromFile(config::coin_path);
    coin_prox = DilatedEntropy::SPtr(new DilatedEntropy(coin));
    coin_egt = EGT::UPtr(new EGT(coin, coin_prox));

    kuhn = GameZerosumPackage::CreateGameFromFile(config::kuhn_path);
    kuhn_prox = DilatedEntropy::SPtr(new DilatedEntropy(kuhn));
    kuhn_egt = EGT::UPtr(new EGT(kuhn, kuhn_prox));

    prsl = GameZerosumPackage::CreateGameFromFile(config::prsl_path);
    prsl_prox = DilatedEntropy::SPtr(new DilatedEntropy(prsl));
    prsl_egt = EGT::UPtr(new EGT(prsl, prsl_prox));

    leduc = GameZerosumPackage::CreateGameFromFile(config::leduc_path, GameName::LEDUC);
    leduc_prox = DilatedEntropy::SPtr(new DilatedEntropy(leduc));
    leduc_egt = EGT::UPtr(new EGT(leduc, leduc_prox));

    int max_sequences = std::max( { coin->num_sequences(Player::P1), coin->num_sequences(Player::P2),
                                    kuhn->num_sequences(Player::P1), kuhn->num_sequences(Player::P2),
                                    prsl->num_sequences(Player::P1), prsl->num_sequences(Player::P2),
                                    leduc->num_sequences(Player::P1), leduc->num_sequences(Player::P2)
                                  });

    utility.resize((unsigned long) max_sequences, 0);
  }

  virtual void TearDown() {
  }

  GameTree::SPtr coin;
  DilatedEntropy::SPtr coin_prox;
  EGT::UPtr coin_egt;

  GameTree::SPtr kuhn;
  DilatedEntropy::SPtr kuhn_prox;
  EGT::UPtr kuhn_egt;

  GameTree::SPtr prsl;
  DilatedEntropy::SPtr prsl_prox;
  EGT::UPtr prsl_egt;

  GameTree::SPtr leduc;
  DilatedEntropy::SPtr leduc_prox;
  EGT::UPtr leduc_egt;

  std::vector<double> utility;
};

TEST_F(EGTTest, egt_coin_excessive_gap_positive) {
  config::mu = 10;
  config::gamma = 1;
  coin_egt = EGT::UPtr(new EGT(coin, coin_prox));
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
  EXPECT_NEAR(efg_solve::config::coin_game_value, coin->GameValue(coin_egt->strategy_profile(), &utility), 0.1);
}
TEST_F(EGTTest, egt_kuhn_game_value) {
  kuhn_egt->Run(100000);
  EXPECT_NEAR(efg_solve::config::kuhn_game_value, kuhn->GameValue(kuhn_egt->strategy_profile(), &utility), 0.1);
}
TEST_F(EGTTest, egt_prsl_game_value) {
  prsl_egt->Run(1000);
  EXPECT_NEAR(efg_solve::config::prsl_game_value, prsl->GameValue(prsl_egt->strategy_profile(), &utility), 0.1);
}
TEST_F(EGTTest, egt_leduc_game_value) {
  leduc_egt->Run(1000);
  EXPECT_NEAR(efg_solve::config::leduc_game_value, leduc->GameValue(leduc_egt->strategy_profile(), &utility), 0.01);
}

TEST_F(EGTTest, egt_leduc_excessive_gap_small_enough) {
  leduc_egt = EGT::UPtr(new EGT(leduc, leduc_prox, 1.5, 1));
  EXPECT_LT(leduc_egt->excessive_gap(), 0.01);
}


TEST_F(EGTTest, leduc_cfr_start) {
  std::vector<double> strategy0 = StrategyIO::ReadStrategyStringIds(config::cfrx_strategy_path, leduc->sequence_names(Player::P1), leduc->num_sequences(Player::P1));
  std::vector<double> strategy1 = StrategyIO::ReadStrategyStringIds(config::cfry_strategy_path, leduc->sequence_names(Player::P2), leduc->num_sequences(Player::P2));
  leduc_egt->WarmStart({{strategy0, strategy1}});
  leduc_egt->Run(1000);
  EXPECT_NEAR(efg_solve::config::leduc_game_value, leduc->GameValue(leduc_egt->strategy_profile(), &utility), 0.01);
}

}
