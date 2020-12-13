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
    void setChar(const int row, const int col, const char val);

    // map each char in str from col to col + str.size()
    void setString(const int row, const int col, const string str);

    // convert num to string and call setString
    void setInt(const int row, const int col, const int num);

    // tile helper methods
    // returns the row, col of the top left vertex in tile # tileNumber
    pair<int, int> getTopLeftCoord(const int tileNumber);

    // update a tile, calls the next three methods
    void updateTile(const shared_ptr<Tile> &tile);
    // set values for a tile
    void setTileNumber(const int tileNumber);
    void setTileValue(const int tileNumber, const int tileValue);
    void setTileResource(const int tileNumber, const string resource);

    // set and remove geese from a tile
    void setGeese(const int tileNumber);
    void removeGeese(const int tileNumber);

    // update the vertex if vertex has a residence
    void updateVertex(const shared_ptr<Vertex> &vertex, const shared_ptr<Builder> &builder);

    // convert edge to a road
    void buildRoad(const shared_ptr<Edge> &edge, const shared_ptr<Builder> &builder);

    friend ostream &operator<<(ostream &out, const TextDisplay &td);
};

ostream &operator<<(ostream &out, const TextDisplay &td);
#endif
