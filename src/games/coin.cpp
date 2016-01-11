//
// Created by Christian Kroer on 9/1/15.
//

#include "coin.h"

int efg_solve::Coin::num_sequences(Player player) const {
  if (player == Player::P1) return 11;
  else return 7;
}

int efg_solve::Coin::num_infosets(Player player) const {
  if (player == Player::P1) return 5;
  else return 3;
}


int efg_solve::Coin::infoset_first_sequence(Player player, int infoset) const {
  return infoset * 2 + 1;
}

int efg_solve::Coin::infoset_last_sequence(Player player, int infoset) const {
  return (infoset+1) * 2;
}

int efg_solve::Coin::infoset_parent_sequence(Player player, int infoset) const {
  if (player == Player::P1) {
    switch (infoset) {
      case 0: return 0;
      case 1: return 1; // Heads
      case 2: return 1; // Heads
      case 3: return 2; // Tails
      case 4: return 2; // Tails
      default:
        printf("infoset does not exist");
        return -1;
    }
  } else {
    switch(infoset) {
      case 0: return 0; // root sequence
      case 1: return 1; // heads guess and p1 bet
      case 2: return 2; // tails guess and p1 bet
      default:
        printf("infoset does not exist");
        return -1;
    }
  }
}


void efg_solve::Coin::UtilityVector(std::vector<double> *strategy, std::vector<double> *utility, Player player) const {
// void efg_solve::Coin::UtilityVector(double *(*strategy), double *(*utility), Player player) const {
  std::fill_n((*utility).begin(), num_sequences(player), 0);

  if (player == Player::P1) {
    double p2h = (*strategy)[1];
    double p2t = (*strategy)[2];
    double p2hbc = (*strategy)[1] * (*strategy)[3];
    double p2hbf = (*strategy)[1] * (*strategy)[4];
    double p2tbc = (*strategy)[2] * (*strategy)[5];
    double p2tbf = (*strategy)[2] * (*strategy)[6];

    // heads heads bet, index 3
    (*utility)[3]  = p2hbc * 3 + p2hbf * 2;
    // heads tails bet, index 5
    (*utility)[5]  = p2tbc * -3 + p2tbf * 2;
    // tails heads bet, index 7
    (*utility)[7]  = p2hbc * -2 + p2hbf * 1;
    // tails tails bet, index 9
    (*utility)[9]  = p2tbc * 2 + p2tbf * 1;


    // heads fold, index 4
    (*utility)[4] = p2h * (-2);
    (*utility)[6] = p2t * (-2);
    // tails fold
    (*utility)[8] = p2h * (-1);
    (*utility)[10] = p2t * (-1);
  } else {
    double p1hhb = (*strategy)[1] * (*strategy)[3];
    double p1hhf = (*strategy)[1] * (*strategy)[4];
    double p1htb = (*strategy)[1] * (*strategy)[5];
    double p1htf = (*strategy)[1] * (*strategy)[6];
    double p1thb = (*strategy)[2] * (*strategy)[7];
    double p1thf = (*strategy)[2] * (*strategy)[8];
    double p1ttb = (*strategy)[2] * (*strategy)[9];
    double p1ttf = (*strategy)[2] * (*strategy)[10];

    // P1 folds
    (*utility)[1]  = p1hhf * 2 + p1thf * 1;
    (*utility)[2]  = p1ttf * 1 + p1htf * 2;
    // heads call, index 3
    (*utility)[3]  = p1hhb * -3 + p1thb * 2;
    // heads fold, index 4
    (*utility)[4]  = p1hhb * -2 + p1thb * -1;
    // tails call, index 5
    (*utility)[5]  = p1ttb * -2 + p1htb * 3;
    // tails fold, index 6
    (*utility)[6]  = p1htb * -2 + p1ttb * -1;
  }
}
