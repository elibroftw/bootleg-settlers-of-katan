#ifndef __GAME_H__
#define __GAME_H__
#include <memory>
#include <string>
#include <vector>

#include "builder.h"
#include "textdisplay.h"
// #include "edge.h"
// #include "resource.h"
#include "vertex.h"

using std::shared_ptr;
using std::string;
using std::vector;

class InvalidLayoutFile{};

class InvalidSaveFile{};

class InvalidArgument{};

class Game {
  int curBuilder;
  int geeseLocation;
  TextDisplay textDisplay;
  vector<shared_ptr<Builder>> builders;
  vector<shared_ptr<Tile>> tiles;
  vector<shared_ptr<Vertex>> vertices;
  vector<vector<shared_ptr<Vertex>>> verticesMap;
  vector<vector<shared_ptr<Vertex>>> edgesMap;
  vector<shared_ptr<Edge>> edges;

  bool validVertex(Vertex &vertex);

 public:
  Game();
  void createBoard();
  void createBoard(string filename);
  void moveGeese(int to);
  void saveGame(string filename);
  void loadGame(string filename);
  void newGame();
  void printBoard();
  void nextTurn();
  void tradeWith(Builder &builder, Resource resource1, Resource resource2);
  void endGame();
  void stealFrom(Builder &builder, Resource resource);
  bool isGameOver();
  void marketTrade(Resource resource1, Resource resource2);
};

#endif
