#include <boost/algorithm/string.hpp>
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
      int node_id = stoi(split_line[0]);
      NodeSetGeneralInformation(split_line);
      if (split_line.size() == 3 || split_line.size() == 5) {
        CreateLeafNode(split_line);
      } else if (node_id < num_chance_histories_) { // nature node
        CreateNatureNode(split_line);
      } else { // player node
        CreatePlayerNode(split_line);
      }
    }
  }
  OrderSequences();
  OrderLeaves();
}


void efg_solve::GameZerosumPackage::UtilityVector(std::vector<double> *strategy, std::vector<double> *utility,
                                                  Player player) const {
  std::fill(std::begin(*utility), std::end(*utility), 0);
  ToSequenceForm(strategy, other_player(player));
  for (int i = 0; i < num_sequences(player); ++i) {
    for (int j = 0; j < opponent_leaf_sequences_[player_id(player)][i].size(); ++j) {
      int opponent_sequence_id = opponent_leaf_sequences_[player_id(player)][i][j];
      double payoff = sequence_payoffs_[player_id(player)][i][j];
      double sequence_prob = (*strategy)[opponent_sequence_id];
      (*utility)[i] += payoff * sequence_prob;
    }
  }
  ToBehavioralStrategy(strategy, other_player(player));
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
  node_leaf_.resize((unsigned long) num_nodes_, false);
  node_parent_node_.resize((unsigned long) num_nodes_, std::numeric_limits<int>::min());
  node_parent_node_[0] = 0;
  node_nature_probability_.resize((unsigned long) num_nodes_, 1);
  node_names_.resize((unsigned long) num_nodes_);
  node_prior_sequence_[0].resize((unsigned long) num_nodes_, 0);
  node_prior_sequence_[1].resize((unsigned long) num_nodes_, 0);
  node_prior_infoset_[0].resize((unsigned long) num_nodes_, -1);
  node_prior_infoset_[1].resize((unsigned long) num_nodes_, -1);
  node_prior_action_[0].resize((unsigned long) num_nodes_, 0);
  node_prior_action_[1].resize((unsigned long) num_nodes_, 0);

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
  information_sets_[0].resize((unsigned long) num_infosets_[0], std::vector<int>());
  information_sets_[1].resize((unsigned long) num_infosets_[1], std::vector<int>());
  infoset_seen_[0].resize((unsigned long) num_infosets_[0], false);
  infoset_seen_[1].resize((unsigned long) num_infosets_[1], false);
  infoset_first_sequence_[0].resize((unsigned long) num_infosets_[0]);
  infoset_first_sequence_[1].resize((unsigned long) num_infosets_[1]);
  infoset_last_sequence_[0].resize((unsigned long) num_infosets_[0]);
  infoset_last_sequence_[1].resize((unsigned long) num_infosets_[1]);
  infoset_parent_sequence_[0].resize((unsigned long) num_infosets_[0]);
  infoset_parent_sequence_[1].resize((unsigned long) num_infosets_[1]);
  infoset_num_actions_[0].resize((unsigned long) num_infosets_[0]);
  infoset_num_actions_[1].resize((unsigned long) num_infosets_[1]);
  infoset_action_names_[0].resize((unsigned long) num_infosets_[0]);
  infoset_action_names_[1].resize((unsigned long) num_infosets_[1]);
  infoset_prior_infoset_[0].resize((unsigned long) num_infosets_[0], 0);
  infoset_prior_infoset_[1].resize((unsigned long) num_infosets_[1], 0);
  infoset_prior_action_[0].resize((unsigned long) num_infosets_[0], 0);
  infoset_prior_action_[1].resize((unsigned long) num_infosets_[1], 0);

  // reset num sequences for keeping track of information set sequence numbers
  num_sequences_[0] = 1;
  num_sequences_[1] = 1;
  // root sequence
  opponent_leaf_sequences_[0].emplace_back();
  opponent_leaf_sequences_[1].emplace_back();
  sequence_payoffs_[0].emplace_back();
  sequence_payoffs_[1].emplace_back();
}

void efg_solve::GameZerosumPackage::CreateLeafNode(vector<string> line) {
  int node_id = stoi(line[0]);
  node_utility_[node_id] = stoi(line[2]);
  node_leaf_[node_id] = true;
}

void efg_solve::GameZerosumPackage::CreatePlayerNode(vector<string> line) {
  int node_id = stoi(line[0]);
  int player = stoi(line[2]);
  int other_player = (player+1)%2;
  int infoset = stoi(line[3]);
  information_sets_[player][infoset].push_back(node_id);
  int num_actions = stoi(line[4]);
  if (!infoset_seen_[player][infoset]){
    infoset_num_actions_[player][infoset] = num_actions;
  }

  int current_sequence = infoset_seen_[player][infoset] ? infoset_first_sequence_[player][infoset] : num_sequences_[player];
  for (int action = 0; action < num_actions; action++) {
    int child_id = stoi(line[6+2* action]);
    node_parent_node_[child_id] = node_id;
    node_prior_infoset_[player][child_id] = infoset;
    node_prior_infoset_[other_player][child_id] = node_prior_infoset_[other_player][node_id];
    node_prior_action_[player][child_id] = action;
    node_prior_action_[other_player][child_id] = node_prior_action_[other_player][node_id];
    if (!infoset_seen_[player][infoset]){
      num_sequences_[player]++;
      infoset_action_names_[player].push_back(line[5+2* action]);
    }
    if (child_id == 822) {
      int temp = 0;
    }
    current_sequence++;
  }
  infoset_seen_[player][infoset] = true;
  if (node_names_[node_id] == "/") {
    root_ = node_id;
  }
}

void efg_solve::GameZerosumPackage::CreateNatureNode(vector<string> line) {
  int node_id = stoi(line[0]);
  node_names_[node_id] = line[1];

  int num_actions = stoi(line[2]);
  double sum = 0;
  for (int i = 0; i < num_actions; i++) {
    //string action_name = line[3+3*i];
    int child_id = stoi(line[4+3*i]);
    node_parent_node_[child_id] = node_id;
    node_prior_infoset_[0][child_id] = node_prior_infoset_[0][node_id];
    node_prior_infoset_[1][child_id] = node_prior_infoset_[1][node_id];
    node_prior_action_[0][child_id] = node_prior_action_[0][node_id];
    node_prior_action_[1][child_id] = node_prior_action_[1][node_id];

    int rem = stoi(line[5+3*i]);
    sum += rem;
  }

  for (int i = 0; i < num_actions; i++) {
    int child_id = stoi(line[4+3*i]);
    int rem = stoi(line[5+3*i]);
    double probability =  rem / sum;
    node_nature_probability_[child_id] = probability;
  }
}


void efg_solve::GameZerosumPackage::NodeSetGeneralInformation(vector<string> line) {
  int node_id = stoi(line[0]);
  node_names_[node_id] = line[1];
  // locate root node
  if (node_names_[node_id] == "/") {
    root_ = node_id;
  }
}

void efg_solve::GameZerosumPackage::OrderSequences() {
  for (int player_id = 0; player_id < 2; player_id++) {
    Player player = player_id == 0 ? Player::P1 : Player::P2;
    parent_sequence_[player_id].resize((unsigned long) num_sequences(player), 0);
    opponent_leaf_sequences_[player_id].resize((unsigned long) num_sequences(player));
    sequence_payoffs_[player_id].resize((unsigned long) num_sequences(player));
  }
  OrderSequences(Player::P1);
  OrderSequences(Player::P2);
}

void efg_solve::GameZerosumPackage::OrderSequences(Player player) {
  int player_idx = player_id(player);
  int current = 1;
  for (int infoset = 0; infoset < num_infosets(player); infoset++) {
    infoset_first_sequence_[player_idx][infoset] = current;
    current += infoset_num_actions_[player_idx][infoset];
    infoset_last_sequence_[player_idx][infoset] = current-1;
    int prior_infoset = node_prior_infoset_[player_idx][information_sets_[player_idx][infoset].front()];
    if (prior_infoset >= 0) {
      int prior_action = node_prior_action_[player_idx][information_sets_[player_idx][infoset].front()];
      infoset_parent_sequence_[player_idx][infoset] = infoset_first_sequence_[player_idx][prior_infoset] + prior_action;
    } else {
      infoset_parent_sequence_[player_idx][infoset] = 0;
    }
  }
}

void efg_solve::GameZerosumPackage::OrderLeaves() {
  for (int node_id = 0; node_id < num_nodes_; ++node_id) {
    if (node_leaf_[node_id]) {
      int prior_infoset1 = node_prior_infoset_[0][node_id];
      int prior_action1 = node_prior_action_[0][node_id];
      int prior_infoset2 = node_prior_infoset_[1][node_id];
      int prior_action2 = node_prior_action_[1][node_id];
      int sequence1 = infoset_first_sequence_[0][prior_infoset1] + prior_action1;
      int sequence2 = infoset_first_sequence_[1][prior_infoset2] + prior_action2;

      double nature_probability = HistoryNatureProbability(node_id);
      double utility = node_utility_[node_id] * nature_probability;

      if (sequence1 == 0 || sequence2 == 0) {
        int temp = 0;
      }
      opponent_leaf_sequences_[0][sequence1].push_back(sequence2);
      sequence_payoffs_[0][sequence1].push_back(utility);

      opponent_leaf_sequences_[1][sequence2].push_back(sequence1);
      sequence_payoffs_[1][sequence2].push_back(-utility);
    }
  }
}


double efg_solve::GameZerosumPackage::HistoryNatureProbability(int node_id) {
  double probability = 1;
  int current = node_id;
  while (current != root_) {
    probability *= node_nature_probability_[current];
    current = node_parent_node_[current];
  }
  return probability;
}
