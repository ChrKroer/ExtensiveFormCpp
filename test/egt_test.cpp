//
// Created by Christian Kroer on 12/12/15.
//

#include <gtest/gtest.h>
#include "../src/solvers/dilated_entropy.h"
#include "../src/solvers/egt.h"
#include "../src/config.h"

namespace efg_solve {
  class EGTTest : public ::testing::Test {
  public:
    virtual void SetUp() {
      coin = new Coin();
      coin_prox = new DilatedEntropy(coin);
      coin_egt = new EGT(coin, coin_prox);

      coin_strategy[0].resize(coin->num_sequences(Player::P1), 0);
      coin_strategy[1].resize(coin->num_sequences(Player::P2), 0);

      int max_actions = std::max(coin->num_sequences(Player::P1), coin->num_sequences(Player::P2));
      coin_utility.resize(max_actions, 0);
      coin_previous.resize(max_actions, 0);
    }

    virtual void TearDown() {
      delete coin;
    }

    Coin *coin;
    DilatedEntropy *coin_prox;
    EGT *coin_egt;

    std::vector<double> coin_strategy[2];
    std::vector<double> coin_utility;
    std::vector<double> coin_previous;
  };

  TEST_F(EGTTest, egt_coin_excessive_gap_positive) {
    config::mu = 100;
    config::gamma = 100;
    EXPECT_GT(coin_egt->excessive_gap(), 0);
  }

  TEST_F(EGTTest, egt_coin_gap_positive) {
    EXPECT_GT(coin_egt->gap(), 0);
    coin_egt->Run(1);
    double gap = coin_egt->gap();
    EXPECT_GT(gap, 0);
  }

  TEST_F(EGTTest, egt_coin_improvent_in_bound) {
    coin_egt->Run(1);
    double gap1 = coin_egt->gap();
    coin_egt->Run(1);
    double gap2 = coin_egt->gap();
    EXPECT_LT(gap2, gap1);
  }

  TEST_F(EGTTest, egt_coin_game_value) {
    coin_egt->Run(1000);

    EXPECT_EQ(efg_solve::config::coin_game_value, coin->GameValue(coin_egt->strategy_profile(), &coin_utility));
  }
}
