#ifndef __TEXT_DISPLAY_H__
#define __TEXT_DISPLAY_H__
#include <iostream>
#include <memory>
#include <vector>

#include "builder.h"
#include "edge.h"
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
    void setChar(int row, int col, char val);
    void setString(int row, int col, string str);
    void setInt(int row, int col, int num);
    // tile helper methods
    // returns the row, col location of the top left "|" of the tileNumber
    pair<int, int> getTopLeftCoord(int tileNumber);
    void setTileNumber(int tileNumber);
    void setTileValue(int tileNumber, int tileValue);
    void setTileResource(int tileNumber, string resource);
    void setGeese(int tileNumber);
    void removeGeese(int tileNumber);
    void updateVertex(shared_ptr<Vertex> &vertex, shared_ptr<Builder> &builder);
    void buildRoad(shared_ptr<Edge> &edge, shared_ptr<Builder> &builder);

    friend ostream &operator<<(ostream &out, const TextDisplay &td);
};

ostream &operator<<(ostream &out, const TextDisplay &td);
#endif
