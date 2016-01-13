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
#include "game_tree.h"

namespace efg_solve {

  class GameZerosumPackage {
  public:
    GameZerosumPackage();

    ~GameZerosumPackage();

    static GameTree::SPtr CreateGameFromFile(std::string filename);

  private:
    static GameTree::SPtr ReadGameInfoLine(const std::vector<std::string> &split_line);
    static void CreateNatureNode(std::vector<std::string> line, GameTree::SPtr tree);
    static void CreateLeafNode(std::vector<std::string> line, GameTree::SPtr tree);
    static void CreatePlayerNode(std::vector<std::string> line, GameTree::SPtr tree);
    static void NodeSetGeneralInformation(std::vector<std::string> line, GameTree::SPtr tree);
  };
}
#endif
