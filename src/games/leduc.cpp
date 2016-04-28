//
// Created by Christian Kroer on 12/17/15.
//

#include "leduc.h"

int efg_solve::Leduc::num_sequences(Player player) const {
  (void)player;
  return 337;
}

int efg_solve::Leduc::num_infosets(Player player) const {
  (void)player;
  return 144;
}


int efg_solve::Leduc::infoset_first_sequence(Player player, int infoset) const {
  (void)player;
  return infoset * 2 + 1;
}

int efg_solve::Leduc::infoset_last_sequence(Player player, int infoset) const {
  (void)player;
  return (infoset+1) * 2;
}

int efg_solve::Leduc::parent_sequence(Player player, int infoset) const {
  (void)player;
  return (infoset+1) * 2;
}

int efg_solve::Leduc::UtilityVector(const std::vector<double> &strategy,
                              std::vector<double> *utility, Player player) const {
  (void)player;
  (void)strategy;
  (void)utility;
  return 0;
}
