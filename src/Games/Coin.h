//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_COIN_H
#define EFG_SOLVERS_COIN_H


#include "Game.h"

class Coin : public Game {
public:
    int nActions(int player) {
        if (player == 1) return 11;
        else if (player == 2) return 12;
        else return 0;
    }
};


#endif //EFG_SOLVERS_COIN_H
