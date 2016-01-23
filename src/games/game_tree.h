//
// Created by Christian Kroer on 1/10/16.
//

#ifndef EFG_SOLVE_GAMETREE_H
#define EFG_SOLVE_GAMETREE_H

#include <array>
#include <string>
#include "game.h"


namespace efg_solve {

enum class NodeType{
  P1 = 0, P2 = 1, NATURE = 2, LEAF = 3
};


class GameTree: public Game {
 public:
  typedef std::shared_ptr<GameTree> SPtr;

  GameTree(int num_chance_histories, int num_combined_player_histories,
           int num_terminal_histories, int num_nodes,
           int num_infosets1, int num_infosets2);
  ~GameTree();

  int num_sequences(Player player) const;

  int num_chance_histories() const { return num_chance_histories_; }

  int num_infosets(Player player) const;

  int infoset_first_sequence(Player player, int infoset) const {
    return infoset_first_sequence_[player_id(player)][infoset];
  }

  int infoset_last_sequence(Player player, int infoset) const {
    return infoset_last_sequence_[player_id(player)][infoset];
  }

  int infoset_parent_sequence(Player player, int infoset) const {
    return infoset_parent_sequence_[player_id(player)][infoset];
  }

  int infoset_num_sequences(Player player, int infoset) const {
    return infoset_num_actions_[player_id(player)][infoset];
  }

  std::vector<std::string> &sequence_names(Player player) { return sequence_names_[player_id(player)]; }

  int UtilityVector(const std::vector<double> &strategy, std::vector<double> *utility, Player player) const;
  int UtilityVector(const std::vector<double> &strategy, std::vector<double> *utility, Player player, double threshold) const;

  // this methods needs to be called after adding all nodes to the game.
  void ComputeGameValues();
  void AddPlayerNode(int node_id, std::string name, Player player,
                     int infoset, std::vector<int> child_ids, std::vector<std::string> sequence_names);

  void AddNatureNode(int node_id, std::string name,
                     std::vector<int> child_ids, std::vector<double> probabilities);

  void AddLeafNode(int node_id, std::string name, int utility);
  void set_root(int node_id) { root_ = node_id; }
 private:
  // index of the root node
  int root_;
  int num_chance_histories_;
  int num_combined_player_histories_;
  int num_terminal_histories_;
  int num_nodes_;
  std::array<int, 2> num_infosets_;
  std::array<int, 2> num_sequences_;

  // individual node information
  std::vector<NodeType> node_type_;
  std::vector<int> node_utility_;
  std::vector<int> node_parent_node_;
  std::vector<int> node_infoset_;
  std::vector<double> node_nature_probability_;
  std::vector<std::string> node_names_;
  std::vector<std::vector<int>> node_children_;

  // individual sequence information
  std::array<std::vector<int>, 2> parent_sequence_;
  // gives the prior sequences leading to the node.
  std::array<std::vector<std::vector<int>>, 2> opponent_leaf_sequences_;
  std::array<std::vector<std::vector<double>>, 2> sequence_payoffs_;
  std::array<std::vector<std::string>, 2> sequence_names_;


  // individual information set information
  // [player][info set index] returns std::vector of indices of the nodes in the information set
  std::array< std::vector<std::vector<int>>, 2> infosets_;
  std::array<std::vector<bool>, 2> infoset_seen_;
  std::array<std::vector<int>, 2> infoset_first_sequence_;
  std::array<std::vector<int>, 2> infoset_last_sequence_;
  std::array<std::vector<int>, 2> infoset_parent_sequence_;
  std::array<std::vector<int>, 2> infoset_num_actions_;
  std::array<std::vector<std::vector<std::string>>, 2> infoset_sequence_names_;



  void init();
  void SetGenericNodeInfo(int node_id, std::string name, NodeType);
  void OrderSequences();
  void OrderSequences(Player player);
  void OrderSequencesRec(int current_node, int parent_sequence_p1, int parent_sequence_p2, double nature_probability);
  void SetLeafData(int node, int sequence_p1, int sequence_p2, double nature_probability);
  int UtilityVectorIterative(const std::vector<double> &strategy, std::vector<double> *utility, Player player) const;
  int UtilityVectorRecursive(const std::vector<double> &strategy,
                             std::vector<double> *utility,
                             Player player,
                             double threshold,
                             int current_node,
                             int parent_sequence_p1,
                             int parent_sequence_p2,
                             double probability
  ) const;
};
}


#endif //EFG_SOLVE_GAMETREE_H
