//
// Created by Christian Kroer on 1/12/16.
//

#include <fstream>
#include <map>
#include "strategy_io.h"
#include "../supportcode/stringfunctions.h"


std::vector<double> StrategyIO::ReadIdStrategyIntegerIds(std::string &filename) {
  std::vector<double> strategy;

  std::ifstream infile(filename);
  size_t id;
  double probability;
  std::string line;
  while (std::getline(infile, line)) {
    if (line[0] == '\'') continue;
    std::vector<std::string> split_line = StringFunctions::SplitWithQuotes(line, ':');
    id = stoi(split_line[0]);
    probability = stof(split_line[1]);
    if (id != strategy.size()) printf("StrategyReader::ReadIdStrategyIntegerIds error: id and vector size do not match. id: %lu, size: %lu", id, strategy.size());
    strategy.push_back(std::max(probability, 0.0));
  }
  infile.close();
  return strategy;
}

std::vector<double> StrategyIO::ReadStrategyStringIds(const std::string &filename,
                                                      const std::vector<std::string> &sequence_names,
                                                      int num_sequences) {
  std::map<std::string, int>  map;
  for (size_t seq = 0; seq < sequence_names.size(); seq++) {
    map[sequence_names[seq]] = seq;
  }
  std::vector<double> strategy((unsigned long) num_sequences);
  strategy[0] = 1;

  std::ifstream infile(filename);
  int id;
  double probability;
  std::string line;
  while (std::getline(infile, line)) {
    if (line[0] == '\'') continue;
    std::vector<std::string> split_line = StringFunctions::SplitWithQuotes(line, ' ');
    id = map[split_line[0]];
    probability = stof(split_line[1]);
    strategy[id] = (std::max(probability, 0.0));
  }
  infile.close();
  return strategy;
}
