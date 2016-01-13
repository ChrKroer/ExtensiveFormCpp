//
// Created by Christian Kroer on 1/12/16.
//

#include <fstream>
#include "strategy_reader.h"
#include "../supportcode/stringfunctions.h"


std::vector<double> StrategyReader::ReadStrategy(std::string &filename) {
  std::vector<double> strategy;

  std::ifstream infile(filename);
  int id;
  double probability;
  std::string line;
  while (std::getline(infile, line)) {
    if (line[0] == '\'') continue;
    std::vector<std::string> split_line = StringFunctions::SplitWithQuotes(line, ':');
    id = stoi(split_line[0]);
    probability = stof(split_line[1]);
    if (id != strategy.size()) printf("StrategyReader::ReadStrategy error: id and vector size do not match. id: %d, size: %lu", id, strategy.size());
    strategy.push_back(std::max(probability, 0.0));
  }
  infile.close();
  return strategy;
}
