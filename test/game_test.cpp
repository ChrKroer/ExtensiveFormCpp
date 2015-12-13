//
// Created by Christian Kroer on 11/19/15.
//

#include "gtest/gtest.h"
#include "../src/games/coin.h"

using namespace efg_solve;

class GameTest : public ::testing::Test {
public:
  virtual void SetUp() {
    coin = new Coin();

    coin_strategy[0].resize(coin->num_sequences(Player::P1), 0);
    coin_strategy[1].resize(coin->num_sequences(Player::P2), 0);

    int max_actions = std::max(coin->num_sequences(Player::P1), coin->num_sequences(Player::P2));
    coin_utility.resize(max_actions, 0);
  }

  virtual void TearDown() {
    delete coin;
  }

  Coin *coin;

  std::vector<double> coin_strategy[2];
  std::vector<double> coin_utility;

};

TEST_F(GameTest, coin_sequences_p1) {
  EXPECT_EQ(coin->num_sequences(Player::P1),11);
}

TEST_F(GameTest, coin_sequences_p2) {
  EXPECT_EQ(coin->num_sequences(Player::P2),7);
}

TEST_F(GameTest, coin_best_response_value1) {
  std::vector<double> utility = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  double br_val = coin->BestResponseValue(Player::P1, &utility);
  EXPECT_EQ(1, br_val);
}

TEST_F(GameTest, coin_best_response_value2) {
  std::vector<double> utility = {0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1};
  double br_val = coin->BestResponseValue(Player::P1, &utility);
  EXPECT_EQ(2, br_val);
}

TEST_F(GameTest, coin_best_response_value3) {
  std::vector<double> utility = {0, 0, 0, 3, 0, 0, 0, 0, 0, 2, 1};
  double br_val = coin->BestResponseValue(Player::P1, &utility);
  EXPECT_EQ(3, br_val);
}
