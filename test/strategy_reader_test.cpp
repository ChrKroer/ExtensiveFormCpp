//
// Created by Christian Kroer on 1/16/16.
//

#include <array>
#include "gtest/gtest.h"
#include "../src/games/game_reader.h"
#include "../src/config.h"
#include "../src/games/strategy_reader.h"

using namespace efg_solve;

class StrategyReaderTest: public ::testing::Test {
 public:
  GameTree::SPtr leduc;


  virtual void SetUp() {
    leduc = GameZerosumPackage::CreateGameFromFile(config::leduc_path, GameName::LEDUC);
  }

  virtual void TearDown() {
  }
};



TEST_F(StrategyReaderTest, leduc_read_cfr_strategy) {
  std::vector<double> strategy = StrategyReader::ReadStrategyStringIds(config::cfrx_strategy_path,
                                                                       leduc->sequence_names(Player::P1),
                                                                       leduc->num_sequences(Player::P1));

  EXPECT_NEAR(0.094278, strategy[1], 0.000001);
}
