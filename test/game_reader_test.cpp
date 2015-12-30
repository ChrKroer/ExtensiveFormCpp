//
// Created by Christian Kroer on 12/27/15.
//

#include <array>
#include "gtest/gtest.h"
#include "../src/games/game_reader.h"

using namespace efg_solve;

class GameReaderTest : public ::testing::Test {
public:
  std::string original_game_files = "/Users/ckroer/Documents/research/zerosum/original_games/";


  std::string coin_path = original_game_files + "coin.txt";
  GameZerosumPackage* coin;
  std::array<std::vector<double>, 2> coin_strategy;
  std::vector<double> coin_utility;


  virtual void SetUp() {
    coin = new GameZerosumPackage(coin_path);

    coin_strategy[0].resize((unsigned long) coin->num_sequences(Player::P1), 0);
    coin_strategy[1].resize((unsigned long) coin->num_sequences(Player::P2), 0);

    int max_actions = std::max(coin->num_sequences(Player::P1), coin->num_sequences(Player::P2));
    coin_utility.resize((unsigned long) max_actions, 0);
  }

  virtual void TearDown() {
    delete coin;
  }

};


TEST_F(GameReaderTest, coin_sequences_p1) {
  EXPECT_EQ(coin->num_sequences(Player::P1),11);
}

TEST_F(GameReaderTest, coin_sequences_p2) {
  EXPECT_EQ(coin->num_sequences(Player::P2),7);
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


TEST_F(GameReaderTest, coin_game_value) {
  coin_strategy[0][1] = 1; // P1 picks heads
  coin_strategy[1][1] = 1; // P2 picks heads
  coin_strategy[0][3] = 1; // P1 bets
  coin_strategy[1][3] = 1; // P2 calls

  EXPECT_EQ(3, coin->GameValue(coin_strategy, &coin_utility));
}

// uniform strategies EV: (0.125)*(-2-2-1-1)+(0.0625)*(3-3-2+2+2+2+1+1) = -0.375
TEST_F(GameReaderTest, coin_game_value2) {
  coin->InitUniform(&coin_strategy[0], Player::P1);
  coin->InitUniform(&coin_strategy[1], Player::P2);
  EXPECT_EQ(-0.375, coin->GameValue(coin_strategy, &coin_utility));
}


TEST_F(GameReaderTest, coin_utility_vector) {
  coin->InitUniform(&coin_strategy[0], Player::P1);
  coin->InitUniform(&coin_strategy[1], Player::P2);


}
