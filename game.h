#ifndef __GAME_H__
#define __GAME_H__
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "builder.h"
#include "dice.h"
#include "edge.h"
#include "resource.h"
#include "textdisplay.h"
#include "tile.h"
#include "vertex.h"

// VM: verticiesMap
#define VM_HEIGHT 11
#define VM_WIDTH 6
// EM: edgesMap
#define EM_HEIGHT 21
#define EM_WIDTH 11
// TD: text display
#define TD_HEIGHT 41
#define TD_WIDTH 54
// constants
#define NUM_BUILDERS 4
#define NUM_TILES 19
#define NUM_VERTICES 54
#define NUM_EDGES 72

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

class InvalidLayoutFile {};

class InvalidSaveFile {};

class Game {
    // keep track of verticies in a grid
    vector<vector<shared_ptr<Vertex>>> verticesMap;
    // keep track of edges in a grid
    vector<vector<shared_ptr<Edge>>> edgesMap;

    int curTurn = -1;
    unsigned geeseLocation = NUM_TILES;
    // this means that the game has passed the beginning stage
    bool gameStarted = false;
    bool gameOver = false;
    Dice dice;
    TextDisplay textDisplay;
    vector<shared_ptr<Builder>> builders{make_shared<Builder>("Blue", 0),
                                         make_shared<Builder>("Red", 1),
                                         make_shared<Builder>("Orange", 2),
                                         make_shared<Builder>("Yellow", 3)};
    vector<shared_ptr<Tile>> tiles;
    vector<shared_ptr<Vertex>> vertices;

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

    // prints the residences the current builder has built
    void printResidences();

    // prints the roads the current builder has built
    void printRoads();

    // prints the board
    void printBoard();

    // start trade from curTurn with builder
    //  returns: whether or not any cin was sucessful
    bool tradeWith(shared_ptr<Builder> &builder, Resource resGive, Resource resTake);
   public:
    Game();
    // creates the board with the given seed
    void createBoard(unsigned seed);

    // create board from a layout file.
    //  if file is invalid, use a seed
    void createBoard(string filename);

    // save the game to filename
    void save(string filename);

    // load from filename
    void load(string filename);

    // begins the game by asking for basement locations
    // returns if EOF was not detected
    bool beginGame();

    // start next players turn.
    //  returns whether turn was sucessful (EOF was not detected)
    bool nextTurn();

    // resets the game
    void reset();

    // returns whether or not the game is over
    bool isGameOver();

    // returns whether or not the beginning phase of the game was completed
    bool hasGameStarted();

    // testing helper. called in test_harness.cc
    void test();
};

#endif
