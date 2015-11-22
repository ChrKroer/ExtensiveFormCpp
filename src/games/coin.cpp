//
// Created by Christian Kroer on 9/1/15.
//

#include "coin.h"

int Coin::num_sequences(int player) const {
  if (player == 1) return 11;
  else return 12;
}

int Coin::num_infosets(int player) const {
  if (player == 1) return 5;
  else return 3;
}


int Coin::infoset_first_action(int player, int infoset) const {
  return infoset * 2;
}

int Coin::infoset_last_action(int player, int infoset) const {
  return (infoset+1) * 2;
}

int Coin::parent_sequence(int player, int infoset) const {
  if (player == 1) {
    switch (infoset) {
      case 0: return 0;
      case 1: return 1; // Heads
      case 2: return 1; // Heads
      case 3: return 2; // Tails
      case 4: return 2; // Tails
    }
  } else {
    switch(infoset) {
      case 0: return 0; // root sequence
      case 1: return 1; // heads guess and p1 bet
      case 2: return 2; // tails guess and p1 bet
    }
  }
}


void Coin::UtilityVector(double *strategy, double *utility_vector, int player) const {
  std::fill(utility_vector, utility_vector + num_sequences(player), 0);

  if (player == 1) {
    double p2hbc = strategy[1] * strategy[3];
    double p2hbf = strategy[1] * strategy[4];
    double p2tbc = strategy[2] * strategy[5];
    double p2tbf = strategy[2] * strategy[6];

    // heads heads bet, index 3
    utility_vector[3]  = p2hbc * -3 + p2hbf * -2;
    // heads tails bet, index 5
    utility_vector[5]  = p2tbc * 3 + p2tbf * -2;
    // tails heads bet, index 7
    utility_vector[7]  = p2hbc * 2 + p2hbf * -1;
    // tails tails bet, index 9
    utility_vector[9]  = p2tbc * -2 + p2tbf * -1;


    // heads fold, index 4
    utility_vector[4] = utility_vector[6] = 2;
    // tails fold
    utility_vector[8] = utility_vector[10] = 2;
  } else {
    double p1hhb = strategy[1] * strategy[3];
    double p1hhf = strategy[1] * strategy[4];
    double p1htb = strategy[1] * strategy[5];
    double p1htf = strategy[1] * strategy[6];
    double p1thb = strategy[2] * strategy[7];
    double p1thf = strategy[2] * strategy[8];
    double p1ttb = strategy[2] * strategy[9];
    double p1ttf = strategy[2] * strategy[10];

    // heads call, index 3
    utility_vector[3]  = p1hhb * 3 + p1thb * -2;
    // heads fold, index 4
    utility_vector[4]  = p1hhb * 2 + p1thb * 1;
    // tails call, index 5
    utility_vector[5]  = p1hhb * 3 + p1thb * 2;
    // tails fold, index 6
    utility_vector[6]  = p1htb * 2 + p1ttb * 1;
  }
}
