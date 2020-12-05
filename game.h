#ifndef __GAME__
#define __GAME__
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "builder.h"
#include "dice.h"
#include "edge.h"
#include "resource.h"
#include "textdisplay.h"
#include "tile.h"
#include "vertex.h"

using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

class InvalidLayoutFile {};

class InvalidSaveFile {};

class InvalidArgument {};

class Game {
    int curBuilder;
    int geeseLocation;
    // this means that the game has passed the beginning stage
    bool gameStarted;
    bool gameOver;
    Dice dice;
    TextDisplay textDisplay;
    vector<shared_ptr<Builder>> builders;
    vector<shared_ptr<Tile>> tiles;
    vector<shared_ptr<Vertex>> vertices;
    vector<vector<shared_ptr<Vertex>>> verticesMap;
    vector<vector<shared_ptr<Edge>>> edgesMap;
    vector<shared_ptr<Edge>> edges;
    vector<int> buildLocations;

    bool isValidVertex(shared_ptr<Vertex> vertex, bool considerEdges = true);
    // to be used in conjunction with geese stealing and resource distribution
    unordered_map<int, int> getBuildersFromTile(int tileNumber);

   public:
    Game();
    void createBoard();
    void createBoard(string filename);
    void moveGeese(int to);
    void saveGame(string filename);
    void loadGame(string filename);
    void beginGame();
    void printBoard();
    // returns whether turn was sucessful (read from cin worked)
    bool nextTurn();
    void tradeWith(Builder &builder, Resource resource1, Resource resource2);
    void resetGame();
    void stealFrom(Builder &builder, Resource resource);
    bool isGameOver();
    bool hasGameStarted();
    void marketTrade(Resource resource1, Resource resource2);
};

#endif
