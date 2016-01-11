#ifndef GAMEH
#define GAMEH
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include <assert.h>
#include <array>
#include "game.h"

namespace efg_solve {

  class GameZerosumPackage : public Game {
  public:
    GameZerosumPackage();

    GameZerosumPackage(std::string filename);

    ~GameZerosumPackage();

    int num_sequences(Player player) const;

    int num_nature_nodes() const;

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


    void UtilityVector(std::vector<double> *strategy, std::vector<double> *utility, Player player) const;

  private:
    // index of the root node
    int root_;
    int num_chance_histories_;
    int num_combined_player_histories_;
    int num_terminal_histories_;
    std::array<int, 2> num_sequences_;
    int num_nodes_;
    std::array<int, 2> num_infosets_;

    // individual node information
    std::vector<int> node_utility_;
    std::vector<bool> node_leaf_;
    std::vector<int> node_parent_node_;
    std::vector<double> node_nature_probability_;
    std::vector<std::string> node_names_;
    std::array<std::vector<int>, 2> node_prior_sequence_;
    std::array<std::vector<int>, 2> node_prior_infoset_;
    std::array<std::vector<int>, 2> node_prior_action_;

    // individual sequence information
    std::array<std::vector<int>, 2> parent_sequence_;
    // gives the prior sequences leading to the node.
    std::array<std::vector<std::vector<int>>, 2> opponent_leaf_sequences_;
    std::array<std::vector<std::vector<double>>, 2> sequence_payoffs_;


    // individual information set information
    // [player][info set index] returns std::vector of indices of the nodes in the information set
    std::array< std::vector<std::vector<int>>, 2> information_sets_;
    std::array<std::vector<bool>, 2> infoset_seen_;
    std::array<std::vector<int>, 2> infoset_first_sequence_;
    std::array<std::vector<int>, 2> infoset_last_sequence_;
    std::array<std::vector<int>, 2> infoset_parent_sequence_;
    std::array<std::vector<int>, 2> infoset_num_actions_;
    std::array<std::vector<std::string>, 2> infoset_action_names_;
    std::array<std::vector<int>, 2> infoset_prior_infoset_;
    std::array<std::vector<int>, 2> infoset_prior_action_;


    bool is_leaf(int node_id) {
      return node_utility_[node_id] != std::numeric_limits<int>::min();
    }

    void ReadGameInfoLine(const std::vector <std::string> &split_line);
    void CreateNatureNode(std::vector <std::string> line);
    void CreateLeafNode(std::vector <std::string> line);
    void CreatePlayerNode(std::vector <std::string> line);
    void CreateGameFromFile(std::string filename);
    void NodeSetGeneralInformation(std::vector<std::string> line);
    void OrderSequences();
    void OrderSequences(Player player);
    void OrderLeaves();
    double HistoryNatureProbability(int node_id);
  };
}
#endif
