//
// Created by Christian Kroer on 9/1/15.
//

#include "coin.h"

int Coin::num_sequences(int player) {
  if (player == 1) return 11;
  else return 12;
}

int Coin::num_infosets(int player) {
  if (player == 1) return 5;
  else return 3;
}

void Coin::UtilityVector(double *relization_plan, double *utility_vector, int player) {

}
