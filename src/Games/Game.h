//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_GAME_H
#define EFG_SOLVERS_GAME_H


class Game {
public:
    virtual int nActions(int player) = 0;

    virtual int nInfoSets(int player) = 0;

    virtual int nActions(int player, int infoSet) = 0;

    virtual bool isLeaf(int history[]) = 0;
};


#endif //EFG_SOLVERS_GAME_H
