//
// Created by Christian Kroer on 12/27/15.
//

#include <array>
#include "gtest/gtest.h"
#include "../src/games/game_reader.h"
#include "../src/config.h"
#include "../src/games/strategy_reader.h"

using namespace efg_solve;

class GameReaderTest : public ::testing::Test {
public:
  GameTree::SPtr coin;
  GameTree::SPtr kuhn;
  GameTree::SPtr leduc;
  std::array<std::vector<double>, 2> strategy;
  std::vector<double> utility;


  virtual void SetUp() {
    coin = GameZerosumPackage::CreateGameFromFile(config::coin_path);
    kuhn = GameZerosumPackage::CreateGameFromFile(config::kuhn_path);
    leduc = GameZerosumPackage::CreateGameFromFile(config::leduc_path, GameName::LEDUC);

    int max_sequences = std::max( { coin->num_sequences(Player::P1), coin->num_sequences(Player::P2),
                                    kuhn->num_sequences(Player::P1), kuhn->num_sequences(Player::P2),
                                    leduc->num_sequences(Player::P1), leduc->num_sequences(Player::P2),
                                  });
    strategy[0].resize((unsigned long) max_sequences, 0);
    strategy[1].resize((unsigned long) max_sequences, 0);
    strategy[0][0] = 1;
    strategy[1][0] = 1;

    utility.resize((unsigned long) max_sequences, 0);
  }

  virtual void TearDown() {
  }
};


TEST_F(GameReaderTest, coin_sequences_p1) {
  EXPECT_EQ(coin->num_sequences(Player::P1),11);
}

TEST_F(GameReaderTest, coin_sequences_p2) {
  EXPECT_EQ(coin->num_sequences(Player::P2),7);
}

TEST_F(GameReaderTest, infoset_parent) {
  EXPECT_EQ(1, coin->infoset_parent_sequence(Player::P2, 1));
  EXPECT_EQ(2, coin->infoset_parent_sequence(Player::P2, 2));
}

TEST_F(GameReaderTest, coin_best_response_value1) {
  std::vector<double> utility = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  double br_val = coin->BestResponseValue(Player::P1, &utility);
  EXPECT_EQ(1, br_val);
}

TEST_F(GameReaderTest, coin_best_response_value2) {
  std::vector<double> utility = {0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1};
  double br_val = coin->BestResponseValue(Player::P1, &utility);
  EXPECT_EQ(2, br_val);
}

TEST_F(GameReaderTest, coin_best_response_value3) {
  std::vector<double> utility = {0, 0, 0, 3, 0, 0, 0, 0, 0, 2, 1};
  double br_val = coin->BestResponseValue(Player::P1, &utility);
  EXPECT_EQ(3, br_val);
}


TEST_F(GameReaderTest, coin_pure_strategy_value) {
  strategy[0][1] = 1; // P1 picks heads
  strategy[1][1] = 1; // P2 picks heads
  strategy[0][3] = 1; // P1 bets
  strategy[1][3] = 1; // P2 calls

  EXPECT_EQ(3, coin->GameValue(&strategy));
}

// uniform strategies EV: (0.125)*(-2-2-1-1)+(0.0625)*(3-3-2+2+2+2+1+1) = -0.375
TEST_F(GameReaderTest, coin_game_value2) {
  coin->InitUniform(&strategy[0], Player::P1);
  coin->InitUniform(&strategy[1], Player::P2);
  EXPECT_EQ(-0.375, coin->GameValue(&strategy, &utility));
}


TEST_F(GameReaderTest, coin_equilibrium_value) {
  strategy[0] = {1, 0.375, 0.625, 0.375, 0, 0.125, 0.25, 0.125, 0.5, 0.625, 0};
  coin->ToBehavioralStrategy(&strategy[0], Player::P1);
  strategy[1] = {1, 0.508929, 0.491071, 0.339286, 0.169643, 0.392857, 0.0982143};
  coin->ToBehavioralStrategy(&strategy[1], Player::P2);

  double game_val = coin->GameValue(&strategy);
  EXPECT_NEAR(config::coin_game_value, game_val, 0.00001);
}

TEST_F(GameReaderTest, kuhn_equilibrium_value) {
  strategy[0] = { 1, 0.666667, 0.333333, 1, 0, 0, 1, 0, 0.666667, 0.666667, 0.333333, 0, 0 };
  kuhn->ToBehavioralStrategy(&strategy[0], Player::P1);
  strategy[1] = { 1, 0.666667, 0.333333, 1, 0, 0, 1, 0, 1, 0.333333, 0.666667, 1, 0 };
  kuhn->ToBehavioralStrategy(&strategy[1], Player::P2);

  double game_val = kuhn->GameValue(&strategy);
  EXPECT_NEAR(config::kuhn_game_value, game_val, 0.00001);
}

TEST_F(GameReaderTest, leduc_equilibrium_value) {
  strategy[0] = StrategyReader::ReadIdStrategyIntegerIds(config::leduc_equilibrium_p1);
  leduc->ToBehavioralStrategy(&strategy[0], Player::P1);
  strategy[1] = StrategyReader::ReadIdStrategyIntegerIds(config::leduc_equilibrium_p2);
  leduc->ToBehavioralStrategy(&strategy[1], Player::P2);
  double game_val = leduc->GameValue(&strategy);
  EXPECT_NEAR(config::leduc_game_value, game_val, 0.00001);
}

