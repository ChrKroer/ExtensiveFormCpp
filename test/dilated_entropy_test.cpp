//
// Created by Christian Kroer on 12/11/15.
//

#include "gtest/gtest.h"
#include "../src/games/coin.h"
#include "../src/solvers/dilated_entropy.h"

using namespace efg_solve;

class DilatedEntropyTest : public ::testing::Test {
public:
  virtual void SetUp() {
    coin = std::make_shared<Coin>();
    coin_prox = std::make_shared<DilatedEntropy>(coin);

    coin_strategy[0].resize(coin->num_sequences(Player::P1), 0);
    coin_strategy[1].resize(coin->num_sequences(Player::P2), 0);

    int max_actions = std::max(coin->num_sequences(Player::P1), coin->num_sequences(Player::P2));
    coin_utility.resize(max_actions, 0);
    coin_previous.resize(max_actions, 0);
  }

  virtual void TearDown() {
  }

  Game::SPtr coin;
  DilatedEntropy::SPtr coin_prox;

  std::vector<double> coin_strategy[2];
  std::vector<double> coin_utility;
  std::vector<double> coin_previous;
};

TEST_F(DilatedEntropyTest, prox_step_leaf_uniform) {
  // Skip non-leaf sequences (by setting i=3).
  for (int i = 3; i < coin_utility.size(); ++i) {
    coin_utility[i] = 1;
  }
  coin_prox->ProxStep(1, Player::P1, &coin_utility, &(coin_strategy[0]));

  for (int j = 1; j < coin->num_infosets(Player::P1); ++j) {
    int first = coin->infoset_first_sequence(Player::P1, j);
    int last = coin->infoset_last_sequence(Player::P1, j);
    for (int i = first; i <= last; ++i) {
      EXPECT_EQ(1.0/(last-first+1), coin_strategy[0][i]);
    }
  }
}

TEST_F(DilatedEntropyTest, prox_step_uniform) {
  coin_prox->ProxStep(1, Player::P1, &coin_utility, &(coin_strategy[0]));

  for (int j = 0; j < coin->num_infosets(Player::P1); ++j) {
    int first = coin->infoset_first_sequence(Player::P1, j);
    int last = coin->infoset_last_sequence(Player::P1, j);
    for (int i = first; i <= last; ++i) {
      EXPECT_EQ(1.0/(last-first+1), coin_strategy[0][i]);
    }
  }
}
