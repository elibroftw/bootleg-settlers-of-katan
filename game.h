#ifndef __GAME_H__
#define __GAME_H__
#include <memory>
#include <string>
#include <vector>

#include "builder.h"
#include "edge.h"
#include "resource.h"
#include "vertex.h"

using std::shared_pointer;
using std::string;
using std::vector;

class InvalidLayoutFile{};

class InvalidSaveFile{};

class Game {
  vector<shared_pointer<Builder>> builders;
  int curBuilder;
  vector<shared_pointer<Tile>> tiles;
  vector<shared_pointer<Vertex>> vertices;
  vector<shared_pointer<Edge>> edges;
  TextDisplay textDisplay;
  int geeseLocation;

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
