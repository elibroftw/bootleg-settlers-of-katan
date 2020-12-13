#ifndef __TEXT_DISPLAY_H__
#define __TEXT_DISPLAY_H__
#include <iostream>
#include <memory>
#include <vector>

#include "builder.h"
#include "edge.h"
#include "tile.h"
#include "vertex.h"

using std::ostream;
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

class TextDisplay {
    vector<vector<char>> board;

   public:
    // create the outlines of the board
    TextDisplay();

    // set row, col to val
    void setChar(int row, int col, char val);

    // map each char in str from col to col + str.size()
    void setString(int row, int col, string str);

    // convert num to string and call setString
    void setInt(int row, int col, int num);

    // tile helper methods
    // returns the row, col location of the top left "|" of a the tile
    //  based on the tileNumber
    pair<int, int> getTopLeftCoord(int tileNumber);

    // update a tile, calls the next three methods
    void updateTile(shared_ptr<Tile>);
    // set values for a tile
    void setTileNumber(int tileNumber);
    void setTileValue(int tileNumber, int tileValue);
    void setTileResource(int tileNumber, string resource);

    // set and remove geese from a tile
    void setGeese(int tileNumber);
    void removeGeese(int tileNumber);

    // update the vertex if vertex has a residence
    void updateVertex(shared_ptr<Vertex> &vertex, shared_ptr<Builder> &builder);

    // convert edge to a road
    void buildRoad(shared_ptr<Edge> &edge, shared_ptr<Builder> &builder);

    friend ostream &operator<<(ostream &out, const TextDisplay &td);
};

ostream &operator<<(ostream &out, const TextDisplay &td);
#endif
