//
// Created by Christian Kroer on 1/10/16.
//

#include "game_tree.h"

efg_solve::GameTree::GameTree(int num_chance_histories,
                              int num_combined_player_histories,
                              int num_terminal_histories,
                              int num_nodes,
                              int num_infosets1,
                              int num_infosets2): num_chance_histories_(num_chance_histories),
                                                  num_combined_player_histories_(num_combined_player_histories),
                                                  num_terminal_histories_(num_terminal_histories),
                                                  num_nodes_(num_nodes)
{
  num_infosets_ = {num_infosets1, num_infosets2};
  init();
}

efg_solve::GameTree::~GameTree() {

}


int efg_solve::GameTree::num_infosets(Player player) const {
  return (int) num_infosets_[player_id(player)];
}

int efg_solve::GameTree::num_sequences(Player player) const {
  return (int) num_sequences_[player_id(player)];
}

void efg_solve::GameTree::UtilityVector(std::vector<double> *strategy, std::vector<double> *utility,
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

void efg_solve::GameTree::init() {
  // individual node information
  // we use the min int value for denoting non-leaf nodes
  node_type_.resize((unsigned long) num_nodes_);
  node_utility_.resize((unsigned long) num_nodes_, std::numeric_limits<int>::min());
  node_parent_node_.resize((unsigned long) num_nodes_, std::numeric_limits<int>::min());
  node_parent_node_[0] = 0;
  node_infoset_.resize((unsigned long) num_nodes_, std::numeric_limits<int>::min());
  node_nature_probability_.resize((unsigned long) num_nodes_, 1);
  node_names_.resize((unsigned long) num_nodes_);
  node_children_.resize((unsigned long) num_nodes_);

  // individual information set information
  infosets_[0].resize((unsigned long) num_infosets_[0], std::vector<int>());
  infosets_[1].resize((unsigned long) num_infosets_[1], std::vector<int>());
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
  infoset_sequence_names_[0].resize((unsigned long) num_infosets_[0]);
  infoset_sequence_names_[1].resize((unsigned long) num_infosets_[1]);

  num_sequences_[0] = 1;
  num_sequences_[1] = 1;
}


void efg_solve::GameTree::ComputeGameValues() {
  // root sequence
  opponent_leaf_sequences_[0].emplace_back();
  opponent_leaf_sequences_[1].emplace_back();
  sequence_payoffs_[0].emplace_back();
  sequence_payoffs_[1].emplace_back();

  OrderSequences();
}

void efg_solve::GameTree::AddPlayerNode(int node_id,
                                        std::string name,
                                        efg_solve::Player player,
                                        int infoset,
                                        std::vector<int> child_ids,
                                        std::vector<std::string> sequence_names) {
  SetGenericNodeInfo(node_id, name, player == Player::P1 ? NodeType::P1 : NodeType::P2);
  infosets_[player_id(player)][infoset].push_back(node_id);
  node_infoset_[node_id] = infoset;

  for (int i = 0; i < child_ids.size(); ++i) {
    int child_id = child_ids[i];
    node_children_[node_id].push_back(child_id);
    if (!infoset_seen_[player_id(player)][infoset]) {
      num_sequences_[player_id(player)]++;
      infoset_sequence_names_[player_id(player)][infoset].push_back(sequence_names[i]);
    }
  }
  if (!infoset_seen_[player_id(player)][infoset]) {
    infoset_seen_[player_id(player)][infoset] = true;
    infoset_num_actions_[player_id(player)][infoset] = (int) child_ids.size();
  }
}

void efg_solve::GameTree::AddNatureNode(int node_id,
                                        std::string name,
                                        std::vector<int> child_ids,
                                        std::vector<double> probabilities) {
  SetGenericNodeInfo(node_id, name, NodeType::NATURE);
  for (int i = 0; i < child_ids.size(); ++i) {
    int child_id = child_ids[i];
    double probability = probabilities[i];
    node_children_[node_id].push_back(child_id);
    node_nature_probability_[child_id] = probability;
  }
}

void efg_solve::GameTree::AddLeafNode(int node_id, std::string name, int utility) {
  node_utility_[node_id] = utility;
  SetGenericNodeInfo(node_id, name, NodeType::LEAF);
}

void efg_solve::GameTree::SetGenericNodeInfo(int node_id, std::string name, efg_solve::NodeType type) {
  node_names_[node_id] = name;
  node_type_[node_id] = type;
}

void efg_solve::GameTree::OrderSequences() {
  for (int player_id = 0; player_id < 2; player_id++) {
    Player player = player_id == 0 ? Player::P1 : Player::P2;
    parent_sequence_[player_id].resize((unsigned long) num_sequences(player), 0);
    opponent_leaf_sequences_[player_id].resize((unsigned long) num_sequences(player));
    sequence_payoffs_[player_id].resize((unsigned long) num_sequences(player));
  }
  OrderSequences(Player::P1);
  OrderSequences(Player::P2);
  OrderSequencesRec(root_, 0, 0, 1);
}

void efg_solve::GameTree::OrderSequences(Player player) {
  int player_idx = player_id(player);
  sequence_names_[player_idx].push_back("");
  int current = 1;
  for (int infoset = 0; infoset < num_infosets(player); infoset++) {
    infoset_first_sequence_[player_idx][infoset] = current;
    for (int i = 0; i < infoset_num_actions_[player_idx][infoset]; i++) {
      sequence_names_[player_idx].push_back(infoset_sequence_names_[player_idx][infoset][i]);
      current++;
    }
    infoset_last_sequence_[player_idx][infoset] = current-1;
  }
}


void efg_solve::GameTree::OrderSequencesRec(int current_node,
                                            int parent_sequence_p1,
                                            int parent_sequence_p2,
                                            double nature_probability)  {
  if (node_type_[current_node] == NodeType::LEAF) {
    SetLeafData(current_node, parent_sequence_p1, parent_sequence_p2, nature_probability);
    return;
  }

  for (int i = 0; i < node_children_[current_node].size(); i++) {
    int child_id = node_children_[current_node][i];
    int sequence_p1 = parent_sequence_p1;
    int sequence_p2 = parent_sequence_p2;
    if (node_type_[current_node] != NodeType::NATURE) {
      Player player = node_type_[current_node] == NodeType::P1 ? Player::P1 : Player::P2;
      int first_seq = node_infoset_[current_node] == -1 ? 0 : infoset_first_sequence_[player_id(player)][node_infoset_[current_node]];
      int player_parent_sequence = player == Player::P1 ? parent_sequence_p1 : parent_sequence_p2;
      int new_sequence = first_seq + i;
      if (player == Player::P1) {
        sequence_p1 = new_sequence;
      } else {
        sequence_p2 = new_sequence;
      }
      parent_sequence_[player_id(player)][new_sequence] = player_parent_sequence;
      infoset_parent_sequence_[player_id(player)][node_infoset_[current_node]] = player_parent_sequence;
    }

    OrderSequencesRec(child_id, sequence_p1, sequence_p2, nature_probability * node_nature_probability_[child_id]);
  }
}
void efg_solve::GameTree::SetLeafData(int node, int sequence_p1, int sequence_p2, double nature_probability) {
  double utility = nature_probability * node_utility_[node];
  opponent_leaf_sequences_[0][sequence_p1].push_back(sequence_p2);
  sequence_payoffs_[0][sequence_p1].push_back(utility);
  opponent_leaf_sequences_[1][sequence_p2].push_back(sequence_p1);
  sequence_payoffs_[1][sequence_p2].push_back(-utility);
}
