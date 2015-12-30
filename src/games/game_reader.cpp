#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include "game_reader.h"
#include "../supportcode/stringfunctions.h"

using namespace std;



efg_solve::GameZerosumPackage::GameZerosumPackage() {

}

efg_solve::GameZerosumPackage::GameZerosumPackage(string filename) {
  CreateGameFromFile(filename);
}

efg_solve::GameZerosumPackage::~GameZerosumPackage() {

}

int efg_solve::GameZerosumPackage::num_nature_nodes() const {
  return num_chance_histories_;
}

int efg_solve::GameZerosumPackage::num_infosets(Player player) const {
  return (int) num_infosets_[player_id(player)];
}

int efg_solve::GameZerosumPackage::num_sequences(Player player) const {
  return (int) num_sequences_[player_id(player)];
}

void efg_solve::GameZerosumPackage::CreateGameFromFile(string filename) {
  ifstream file(filename);

  string line;
  while (std::getline(file, line)) {
    boost::trim_if(line, boost::is_any_of("\t "));
    std::vector<std::string> split_line = StringFunctions::SplitWithQuotes(line, ' ');
    if (line[0] == '#') {
      continue;
    } else if (!StringFunctions::is_number(split_line[0])) {
      ReadGameInfoLine(split_line);
    } else {
      NodeSetGeneralInformation(split_line);
      if (split_line.size() == 3 || split_line.size() == 5) {
        CreateLeafNode(split_line);
      } else if (split_line[5][0] == 'n') { // nature node
        CreateNatureNode(split_line);
      } else { // player node
        CreatePlayerNode(split_line);
      }
    }
  }
}

void efg_solve::GameZerosumPackage::ReadGameInfoLine(const vector<string>& split_line) {
  // game constants
  num_chance_histories_ = stoi(split_line[1]);
  num_combined_player_histories_ = stoi(split_line[2]);
  num_terminal_histories_ = stoi(split_line[3]);
  num_sequences_[0] =  stoi(split_line[4]);
  num_sequences_[1] =  stoi(split_line[5]);
  num_nodes_ =  (stoi(split_line[5]) + 1);
  num_infosets_[0] = stoi(split_line[7]);
  num_infosets_[1] = stoi(split_line[8]);

  // individual node information
  // we use the min int value for denoting non-leaf nodes
  node_utility_.resize((unsigned long) num_nodes_, std::numeric_limits<int>::min());
  node_parent_node_.resize((unsigned long) num_nodes_, std::numeric_limits<int>::min());
  node_parent_node_[0] = 0;
  node_nature_probability_.resize((unsigned long) num_nodes_, 1);
  node_names_.resize((unsigned long) num_nodes_);
  node_prior_sequence_[0].resize((unsigned long) num_nodes_, 0);
  node_prior_sequence_[1].resize((unsigned long) num_nodes_, 0);

  // individual sequence information
//  parent_sequence_[0].reserve((unsigned long) num_sequences_[0]);
//  parent_sequence_[1].reserve((unsigned long) num_sequences_[1]);
//  opponent_leaf_sequences_[0].reserve((unsigned long) num_sequences_[0]);
//  opponent_leaf_sequences_[1].reserve((unsigned long) num_sequences_[1]);
//  sequence_payoffs_[0].reserve((unsigned long) num_sequences_[0]);
//  sequence_payoffs_[1].reserve((unsigned long) num_sequences_[1]);
//  sequence_name_[0].reserve((unsigned long) num_sequences_[0]);
//  sequence_name_[1].reserve((unsigned long) num_sequences_[1]);

  // individual information set information
  information_sets_[0].resize((unsigned long) num_infosets_[0]);
  information_sets_[1].resize((unsigned long) num_infosets_[1]);
  information_sets_seen_[0].resize((unsigned long) num_infosets_[0], false);
  information_sets_seen_[1].resize((unsigned long) num_infosets_[1], false);
  information_set_first_sequence_[0].resize((unsigned long) num_infosets_[0]);
  information_set_first_sequence_[1].resize((unsigned long) num_infosets_[1]);
  information_set_last_sequence_[0].resize((unsigned long) num_infosets_[0]);
  information_set_last_sequence_[1].resize((unsigned long) num_infosets_[1]);
  information_set_parent_sequence_[0].resize((unsigned long) num_infosets_[0]);
  information_set_parent_sequence_[1].resize((unsigned long) num_infosets_[1]);

  // reset num sequences for keeping track of information set sequence numbers
  num_sequences_[0] = 0;
  num_sequences_[1] = 0;
}

void efg_solve::GameZerosumPackage::CreateLeafNode(vector<string> line) {
  int node_id = stoi(line[0]);
  node_utility_[node_id] = stoi(line[2]);
  // create data structures for sparse iteration over utilities
  opponent_leaf_sequences_[0][node_prior_sequence_[0][node_id]].push_back(node_prior_sequence_[1][node_id]);
  sequence_payoffs_[0][node_prior_sequence_[0][node_id]].push_back(node_utility_[node_id]);
  opponent_leaf_sequences_[1][node_prior_sequence_[1][node_id]].push_back(node_prior_sequence_[0][node_id]);
  sequence_payoffs_[1][node_prior_sequence_[1][node_id]].push_back(node_utility_[node_id]);
}

void efg_solve::GameZerosumPackage::CreatePlayerNode(vector<string> line) {
  int node_id = stoi(line[0]);
  int player = stoi(line[2]);
  int other_player = (player+1)%2;
  int infoset = stoi(line[3]);
  node_nature_probability_[node_id] = node_nature_probability_[node_parent_node_[node_id]];
  information_sets_[player][infoset].push_back(node_id);
  if (!information_sets_seen_[player-1][infoset]){
    information_set_parent_sequence_[player][infoset] = node_prior_sequence_[player][node_id];
    information_set_first_sequence_[player][infoset] = num_sequences_[player];
  }

  int num_actions = stoi(line[4]);
  for (int i = 0; i < num_actions; i++) {
    int child_id = stoi(line[6+2*i]);
    node_parent_node_[child_id] = node_id;
    node_nature_probability_[child_id] = node_nature_probability_[node_id];
    node_prior_sequence_[player][child_id] = num_sequences_[player];
    node_prior_sequence_[other_player][child_id] = node_prior_sequence_[other_player][node_id];
    if (!information_sets_seen_[player-1][infoset]){
      sequence_name_[player].push_back(line[5+2*i]);
      parent_sequence_[player].push_back(node_prior_sequence_[player][node_id]);
      opponent_leaf_sequences_[player].emplace_back();
      sequence_payoffs_[player].emplace_back();
      num_sequences_[player]++;
    }
  }
  if (!information_sets_seen_[player-1][infoset]){
    information_set_last_sequence_[player][infoset] = num_sequences_[player];
  }
  information_sets_seen_[player-1][infoset] = true;
  if (node_names_[node_id] == "/") {
    //cout << "found root: " << node_id << endl;
    root_ = node_id;
  }
}

void efg_solve::GameZerosumPackage::CreateNatureNode(vector<string> line) {
  int node_id = stoi(line[0]);
  node_names_[node_id] = line[1];

  int num_actions = stoi(line[2]);
  double sum = 0;
  for (int i = 0; i < num_actions; i++) {
    string action_name = line[3+3*i];
    int child_id = stoi(line[4+3*i]);
    node_parent_node_[child_id] = node_id;
    node_prior_sequence_[0][child_id] = node_prior_sequence_[0][node_id];
    node_prior_sequence_[1][child_id] = node_prior_sequence_[1][node_id];

    int rem = stoi(line[5+3*i]);
    sum += rem;
  }

  for (int i = 0; i < num_actions; i++) {
    int child_id = stoi(line[4+3*i]);
    int rem = stoi(line[5+3*i]);
    double probability =  rem / sum;
    node_nature_probability_[child_id] = node_nature_probability_[node_id] * probability;
  }

  // locate root node
  if (node_names_[node_id] == "/") {
    root_ = node_id;
  }
}


void efg_solve::GameZerosumPackage::NodeSetGeneralInformation(vector<string> line) {
  int node_id = stoi(line[0]);
  node_names_[node_id] = line[1];
}

void efg_solve::GameZerosumPackage::UtilityVector(const std::vector<double> &strategy, std::vector<double> *utility,
                                                  Player player) const {
  for (int i = 0; i < num_sequences(player); ++i) {
    for (int j = 0; j < opponent_leaf_sequences_[player_id(player)].size(); ++j) {
      double sequence_prob;
    }
  }
}
