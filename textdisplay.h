#ifndef __TEXT_DISPLAY__
#define __TEXT_DISPLAY__
#include <iostream>
#include <vector>

using std::ostream;
using std::pair;
using std::string;
using std::vector;

class TextDisplay {
    vector<vector<char>> board;


   public:
    // create the outlines of the board
    TextDisplay();
    void setChar(int x, int y, char c);
    void setString(int x, int y, string c);
    void setInt(int x, int y, int v);
    // tile helper methods
    // returns the location of the top left "|" of the tileNumber
    pair<int, int> getTopLeftCoord(int tileNumber);
    void setTileNumber(int tileNumber);
    void setTileValue(int tileNumber, int tileValue);
    void setResourceName(int tileNumber, string resource);
    void setGeese(int tileNumber);
    void removeGeese(int tileNumber);
    // todo: update tile resource, geese, value, number
    // maybe each tile has an x and y coordinate equal to top-left?
    friend ostream &operator<<(ostream &out, const TextDisplay &td);
};

ostream &operator<<(ostream &out, const TextDisplay &td);
#endif
