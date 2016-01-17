//
// Created by Christian Kroer on 1/12/16.
//

#ifndef EFG_SOLVE_STRATEGY_READER_H
#define EFG_SOLVE_STRATEGY_READER_H


#include <vector>
#include <string>

class StrategyReader {
 public:
  static std::vector<double> ReadIdStrategyIntegerIds(std::string &filename);
  /*
   * sequence_names should be a vector where the element at index i contains the name of sequence i.
   */
  static std::vector<double> ReadStrategyStringIds(const std::string &filename,
                                                          const std::vector<std::string> &sequence_names,
                                                          int num_sequences);
};


#endif //EFG_SOLVE_STRATEGY_READER_H
