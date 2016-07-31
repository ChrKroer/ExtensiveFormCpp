//
// Created by Christian Kroer on 9/1/15.
//

#ifndef EFG_SOLVERS_GAME_H
#define EFG_SOLVERS_GAME_H


#include <vector>

namespace efg_solve {

  enum class Player {
    P1 = 0, P2 = 1
  };

  class Game {
  public:
    typedef std::shared_ptr<Game> SPtr;

    virtual ~Game() = default;

    // return the total number of sequences available to the player.
    virtual int num_sequences(Player player) const = 0;

    virtual int num_infosets(Player player) const = 0;

    virtual int infoset_first_sequence(Player player, int infoset) const = 0;

    virtual int infoset_last_sequence(Player player, int infoset) const = 0;

    virtual int infoset_parent_sequence(Player player, int infoset) const = 0;

    int player_id(Player player) const { if (player == Player::P1) { return 0; } { return 1;
}}

    // returned value is the amount the intput player can increase their utility by best response.
    double BestResponseValue(Player player, std::vector<double> *utility) const;
    double MaxRegret(const std::array<std::vector<double>, 2> &strategy_profile) const {
      double x,y;
      return MaxRegret(strategy_profile, x, y);
    }
    double MaxRegret(const std::array<std::vector<double>, 2> &strategy_profile, double &regret1, double &regret2) const;
    double GameValue(const std::array<std::vector<double>, 2> &strategy_profile) const;
    // additional version with optionally given utility vector to avoid allocation
    double GameValue(const std::array<std::vector<double>, 2> &strategy_profile, std::vector<double> *utility) const;
    // same as above, but allows choice of which player is used for computing the utility vector.
    double GameValue(const std::array<std::vector<double>, 2> &strategy_profile, std::vector<double> *utility, Player player) const;
    // converts the strategy to a sequence form strategy. Assumes that a behavioral strategy is given.
    void ToSequenceForm(std::vector<double> *strategy, Player player) const;
    // converts the strategy to a behavioral strategy. Assumes that a sequence form strategy is given.
    void ToBehavioralStrategy(std::vector<double> *strategy, Player player) const;
    void InitUniform(std::vector<double> *strategy, Player player) const;
    /**
     * Fills in the utility vector of player using the strategy of the opposing player.
     * Expected values are computed such that the input player wishes to maximize the returned values.
     */
    virtual int UtilityVector(const std::vector<double> &opponent_strategy, std::vector<double> *utility, Player player) const = 0;
    virtual int UtilityVector(const std::vector<double> &opponent_strategy, std::vector<double> *utility, Player player, double threshold) const = 0;

    Player other_player(Player player) const { if (player == Player::P1) { return Player::P2; } { return Player::P1;
}}
  };


}  // namespace efg_solve

#endif //EFG_SOLVERS_GAME_H
