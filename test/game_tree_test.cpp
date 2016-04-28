//
// Created by Christian Kroer on 1/22/16.
//

#include <array>
#include "gtest/gtest.h"
#include "../src/games/game_reader.h"
#include "../src/config.h"
#include "../src/games/strategy_io.h"

using namespace efg_solve;

class GameTreeTest : public ::testing::Test {
 public:
  GameTree::SPtr coin;
  GameTree::SPtr kuhn;
  GameTree::SPtr leduc;
  GameTree::SPtr leduckj1;
  std::array<std::vector<double>, 2> strategy;
  std::vector<double> utility;


  virtual void SetUp() {
    coin = GameZerosumPackage::CreateGameFromFile(config::coin_path);
    kuhn = GameZerosumPackage::CreateGameFromFile(config::kuhn_path);
    leduc = GameZerosumPackage::CreateGameFromFile(config::leduc_path, GameName::LEDUC);
    leduckj1 = GameZerosumPackage::CreateGameFromFile(config::leduckj1_path, GameName::LEDUC);

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

TEST_F(GameTreeTest, coin_equilibrium_zero_regret) {
  strategy[0] = {1, 0.375, 0.625, 0.375, 0, 0.125, 0.25, 0.125, 0.5, 0.625, 0};
  coin->ToBehavioralStrategy(&strategy[0], Player::P1);
  //coin->PrintStrategy(Player::P1, strategy[0]);
  strategy[1] = {1, 0.508929, 0.491071, 0.339286, 0.169643, 0.392857, 0.0982143};
  coin->ToBehavioralStrategy(&strategy[1], Player::P2);
  //coin->PrintStrategy(Player::P2, strategy[1]);

  double max_regret = coin->MaxRegret(strategy);
  EXPECT_NEAR(0, max_regret, 0.00001);
}

TEST_F(GameTreeTest, kuhn_recursive_iterative_agree) {
  strategy[0] = { 1, 0.666667, 0.333333, 1, 0, 0, 1, 0, 0.666667, 0.666667, 0.333333, 0, 0 };
  kuhn->ToBehavioralStrategy(&strategy[0], Player::P1);
  //kuhn->PrintStrategy(Player::P1, strategy[0]);
  strategy[1] = { 1, 0.666667, 0.333333, 1, 0, 0, 1, 0, 1, 0.333333, 0.666667, 1, 0 };
  kuhn->ToBehavioralStrategy(&strategy[1], Player::P2);
  kuhn->PrintStrategy(Player::P2, strategy[1]);

  std::vector<double> utility1((unsigned long) kuhn->num_sequences(Player::P1));
  std::vector<double> utility2((unsigned long) kuhn->num_sequences(Player::P1));
  kuhn->UtilityVector(strategy[1], &utility1, Player::P1);
  kuhn->UtilityVector(strategy[1], &utility2, Player::P1, 0.01);
  for (int i = 0; i < utility1.size(); ++i) {
    EXPECT_NEAR(utility1[i], utility2[i], 0.001);
  }
}

TEST_F(GameTreeTest, leduc_recursive_iterative_agree) {
  strategy[0] = StrategyIO::ReadIdStrategyIntegerIds(config::leduc_equilibrium_p1);
  leduc->ToBehavioralStrategy(&strategy[0], Player::P1);
  strategy[1] = StrategyIO::ReadIdStrategyIntegerIds(config::leduc_equilibrium_p2);
  leduc->ToBehavioralStrategy(&strategy[1], Player::P2);

  std::vector<double> utility1((unsigned long) leduc->num_sequences(Player::P1));
  std::vector<double> utility2((unsigned long) leduc->num_sequences(Player::P1));
  leduc->UtilityVector(strategy[1], &utility1, Player::P1);
  leduc->UtilityVector(strategy[1], &utility2, Player::P1, 0.01);
  for (int i = 0; i < utility1.size(); ++i) {
    EXPECT_NEAR(utility1[i], utility2[i], 0.001);
  }
}

TEST_F(GameTreeTest, leduckj1_equilibrium_value) {
  strategy[0] = StrategyIO::ReadIdStrategyIntegerIds(config::leduckj1_equilibrium_p1);
  leduc->ToBehavioralStrategy(&strategy[0], Player::P1);
  strategy[1] = StrategyIO::ReadIdStrategyIntegerIds(config::leduckj1_equilibrium_p2);
  leduc->ToBehavioralStrategy(&strategy[1], Player::P2);
  double game_val = leduckj1->GameValue(strategy);
  EXPECT_NEAR(config::leduckj1_game_value, game_val, 0.00001);
}

TEST_F(GameTreeTest, leduckj1_equilibrium_zero_regret) {
  strategy[0] = StrategyIO::ReadIdStrategyIntegerIds(config::leduckj1_equilibrium_p1);
  leduc->ToBehavioralStrategy(&strategy[0], Player::P1);
  strategy[1] = StrategyIO::ReadIdStrategyIntegerIds(config::leduckj1_equilibrium_p2);
  leduc->ToBehavioralStrategy(&strategy[1], Player::P2);

  double regret1, regret2;
  double max_regret = leduckj1->MaxRegret(strategy, regret1, regret2);

  EXPECT_NEAR(0, regret1, 0.00001);
  EXPECT_NEAR(0, regret2, 0.00001);
}
