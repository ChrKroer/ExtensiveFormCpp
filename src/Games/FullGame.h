//
// Created by Christian Kroer on 11/19/15.
//

#ifndef EFG_SOLVE_FULLGAME_H
#define EFG_SOLVE_FULLGAME_H

#include <string>

class FullGame {
public:
  void ReadFromZerosumFile(std::string path);

private:
  int num_info_sets[2];
  int num_sequences[2];

  int nodes[];
};


#endif //EFG_SOLVE_FULLGAME_H
