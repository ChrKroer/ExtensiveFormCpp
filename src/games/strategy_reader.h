//
// Created by Christian Kroer on 1/12/16.
//

#ifndef EFG_SOLVE_STRATEGY_READER_H
#define EFG_SOLVE_STRATEGY_READER_H


#include <vector>
#include <string>

class StrategyReader {
 public:
  static std::vector<double> ReadStrategy(std::string &filename);
};


#endif //EFG_SOLVE_STRATEGY_READER_H
