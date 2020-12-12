#ifndef __GAME_H__
#define __GAME_H__
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

// VM = verticiesMap
#define VM_HEIGHT 11
#define VM_WIDTH 6
// EM = edgesMap
#define EM_HEIGHT 21
#define EM_WIDTH 11
// text display height and width
#define TD_HEIGHT 41
#define TD_WIDTH 54

#define NUM_BUILDERS 4

using std::pair;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;
using std::make_shared;

class InvalidLayoutFile {};

class InvalidSaveFile {};

class InvalidArgument {};

class Game {
    int curTurn;
    unsigned geeseLocation;
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
    // vector<vector<shared_ptr<Edge>>> edgesMap;
    vector<shared_ptr<Edge>> edges;
    vector<int> resLocations;
    vector<int> roadLocations;

    bool isValidVertex(shared_ptr<Vertex> vertex, bool considerEdges = true);
    // checks if edge is valid to build a road on for curTurn builder
    bool isValidEdge(shared_ptr<Edge> edge);
    // to be used in conjunction with geese stealing and resource distribution
    unordered_map<int, int> getBuildersFromTile(int tileNumber);

    // clears and ignores cin
    void resetCin();

    // prints the current status of all builders in order from builder 0 to 3
    void printStatus();

    // prints the board
    void printBoard();

    // start trade from curTurn with builder, returns whether or not any cin was sucessful
    bool tradeWith(shared_ptr<Builder> &builder, Resource resGive, Resource resTake);

    // TODO: remove
    void stealFrom(shared_ptr<Builder> &builder, Resource resource);

    // TODO: bonus feature
    void marketTrade(Resource resource1, Resource resource2);

   public:
    Game();
    void createBoard(unsigned seed);
    void createBoard(string filename);
    void saveGame(string filename);
    void loadGame(string filename);

    // begins the game by asking for basement locations
    // returns if EOF was not detected
    bool beginGame();

    // start next players turn.
    //  returns whether turn was sucessful (EOF was not detected)
    bool nextTurn();

    // resets the game
    void resetGame();

    // returns whether or not the game is over
    bool isGameOver();

    // returns whether or not the beginning phase of the game was completed
    bool hasGameStarted();

    // testing helper
    void test();
};

#endif
