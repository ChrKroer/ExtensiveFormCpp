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
