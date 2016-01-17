#include <boost/algorithm/string.hpp>
#include <sstream>
#include "game_reader.h"
#include "../supportcode/stringfunctions.h"
#include "game_tree.h"

using namespace std;



efg_solve::GameZerosumPackage::GameZerosumPackage() {

}

efg_solve::GameZerosumPackage::~GameZerosumPackage() {

}



efg_solve::GameTree::SPtr efg_solve::GameZerosumPackage::CreateGameFromFile(string filename, GameName game) {
  std::shared_ptr<GameTree> tree;
  ifstream file(filename);

  string line;
  while (std::getline(file, line)) {
    boost::trim_if(line, boost::is_any_of("\t "));
    std::vector<std::string> split_line = StringFunctions::SplitWithQuotes(line, ' ');
    if (line[0] == '#') {
      continue;
    } else if (!StringFunctions::is_number(split_line[0])) {
      tree = ReadGameInfoLine(split_line);
    } else {
      int node_id = stoi(split_line[0]);
      NodeSetGeneralInformation(split_line, tree);
      if (split_line.size() == 3 || split_line.size() == 5) {
        CreateLeafNode(split_line, tree);
      } else if (node_id < tree->num_chance_histories()) { // nature node
        CreateNatureNode(split_line, tree);
      } else { // player node
        CreatePlayerNode(split_line, tree, game);
      }
    }
  }
  tree->ComputeGameValues();
  return tree;
}



efg_solve::GameTree::SPtr efg_solve::GameZerosumPackage::ReadGameInfoLine(const vector<string>& split_line) {
  // game constants
  int num_chance_histories_ = stoi(split_line[1]);
  int num_combined_player_histories_ = stoi(split_line[2]);
  int num_terminal_histories_ = stoi(split_line[3]);
  int num_nodes_ =  (stoi(split_line[5]) + 1);
  int num_infosets0 = stoi(split_line[7]);
  int num_infosets1 = stoi(split_line[8]);
  return std::make_shared<GameTree>(num_chance_histories_, num_combined_player_histories_,
                                     num_terminal_histories_, num_nodes_,
                                     num_infosets0, num_infosets1);
}

void efg_solve::GameZerosumPackage::CreateLeafNode(vector<string> line, std::shared_ptr<GameTree> tree) {
  int node_id = stoi(line[0]);
  std::string name = line[1];
  int utility = stoi(line[2]);
  tree->AddLeafNode(node_id, name, utility);
}

void efg_solve::GameZerosumPackage::CreatePlayerNode(vector<string> line, std::shared_ptr<GameTree> tree, GameName game) {
  int node_id = stoi(line[0]);
  std::string name = line[1];
  Player player = stoi(line[2]) == 0 ? Player::P1 : Player::P2;
  int infoset = stoi(line[3]);
  int num_actions = stoi(line[4]);

  std::vector<int> child_ids;
  std::vector<std::string> sequence_names;
  for (int action = 0; action < num_actions; action++) {
    int child_id = stoi(line[6+2* action]);
    std::string action_name = line[5+2* action];
    if (game == GameName::LEDUC) {
      sequence_names.push_back(GetLeducSequenceName(name, action_name, player));
    } else {
      sequence_names.push_back(action_name);
    }
    child_ids.push_back(child_id);
  }

  tree->AddPlayerNode(node_id, name, player, infoset, child_ids, sequence_names);
}


std::string efg_solve::GameZerosumPackage::GetLeducSequenceName(std::string nodename, std::string actionname, Player player) {
  for (int i = 0; i < nodename.length(); ++i) {
    if (nodename[i] == 'r') nodename[i] = '1';
  }

  std::stringstream ss;
  ss << (player == Player::P1 ? nodename[1] : nodename[2]);
  for (int i = 4; i < nodename.length(); i++ ) {
    if (nodename[i] != '/') {
      ss << nodename[i];
    }
  }
  // if (nodename.length() > 4) ss << nodename.substr(4);
  ss << (actionname == "r" ? "1" : actionname);
  return ss.str();
}


void efg_solve::GameZerosumPackage::CreateNatureNode(vector<string> line, std::shared_ptr<GameTree> tree) {
  int node_id = stoi(line[0]);
  std::string name = line[1];
  int num_actions = stoi(line[2]);
  std::vector<int> child_ids;
  std::vector<double> probabilities;
  double sum = 0;
  for (int i = 0; i < num_actions; i++) {
    //string action_name = line[3+3*i];
    int child_id = stoi(line[4+3*i]);
    child_ids.push_back(child_id);

    int rem = stoi(line[5+3*i]);
    sum += rem;
  }

  for (int i = 0; i < num_actions; i++) {
    int rem = stoi(line[5+3*i]);
    double probability =  rem / sum;
    probabilities.push_back(probability);
  }

  tree->AddNatureNode(node_id, name, child_ids, probabilities);
}


void efg_solve::GameZerosumPackage::NodeSetGeneralInformation(vector<string> line, std::shared_ptr<GameTree> tree) {
  int node_id = stoi(line[0]);
  std::string name = line[1];
  // locate root node
  if (name == "/") {
    tree->set_root(node_id);
  }
}

//void efg_solve::GameZerosumPackage::OrderSequences() {
//  for (int player_id = 0; player_id < 2; player_id++) {
//    Player player = player_id == 0 ? Player::P1 : Player::P2;
//    parent_sequence_[player_id].resize((unsigned long) num_sequences(player), 0);
//    opponent_leaf_sequences_[player_id].resize((unsigned long) num_sequences(player));
//    sequence_payoffs_[player_id].resize((unsigned long) num_sequences(player));
//  }
//  OrderSequences(Player::P1);
//  OrderSequences(Player::P2);
//}
//
//void efg_solve::GameZerosumPackage::OrderSequences(Player player) {
//  int player_idx = player_id(player);
//  int current = 1;
//  for (int infoset = 0; infoset < num_infosets(player); infoset++) {
//    infoset_first_sequence_[player_idx][infoset] = current;
//    current += infoset_num_actions_[player_idx][infoset];
//    infoset_last_sequence_[player_idx][infoset] = current-1;
//    int prior_infoset = node_prior_infoset_[player_idx][information_sets_[player_idx][infoset].front()];
//    if (prior_infoset >= 0) {
//      int prior_action = node_prior_action_[player_idx][information_sets_[player_idx][infoset].front()];
//      infoset_parent_sequence_[player_idx][infoset] = infoset_first_sequence_[player_idx][prior_infoset] + prior_action;
//    } else {
//      infoset_parent_sequence_[player_idx][infoset] = 0;
//    }
//  }
//}
//
//void efg_solve::GameZerosumPackage::OrderLeaves() {
//  for (int node_id = 0; node_id < num_nodes_; ++node_id) {
//    if (node_leaf_[node_id]) {
//      int prior_infoset1 = node_prior_infoset_[0][node_id];
//      int prior_action1 = node_prior_action_[0][node_id];
//      int prior_infoset2 = node_prior_infoset_[1][node_id];
//      int prior_action2 = node_prior_action_[1][node_id];
//      int sequence1 = infoset_first_sequence_[0][prior_infoset1] + prior_action1;
//      int sequence2 = infoset_first_sequence_[1][prior_infoset2] + prior_action2;
//
//      double nature_probability = HistoryNatureProbability(node_id);
//      double utility = node_utility_[node_id] * nature_probability;
//
//      if (sequence1 == 0 || sequence2 == 0) {
//        int temp = 0;
//      }
//      opponent_leaf_sequences_[0][sequence1].push_back(sequence2);
//      sequence_payoffs_[0][sequence1].push_back(utility);
//
//      opponent_leaf_sequences_[1][sequence2].push_back(sequence1);
//      sequence_payoffs_[1][sequence2].push_back(-utility);
//    }
//  }
//}
//
//
//double efg_solve::GameZerosumPackage::HistoryNatureProbability(int node_id) {
//  double probability = 1;
//  int current = node_id;
//  while (current != root_) {
//    probability *= node_nature_probability_[current];
//    current = node_parent_node_[current];
//  }
//  return probability;
//}
