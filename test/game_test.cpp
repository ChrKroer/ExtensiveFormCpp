//
// Created by Christian Kroer on 11/19/15.
//

#include "gtest/gtest.h"
#include "../src/games/coin.h"

using namespace efg_solve;

class GameTest : public ::testing::Test {
  Coin coin;

};

TEST_F(GameTest, coin_sequences_p1) {
  EXPECT_EQ(coin.num_sequences(Player::P1),11);
}

TEST_F(GameTest, coin_sequences_p2) {
  EXPECT_EQ(coin.num_sequences(Player::P2),7);
}
