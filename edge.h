#ifndef __EDGE__
#define __EDGE__
#include <memory>

#include "builder.h"

using std::shared_ptr;

class Edge {
    int owner;
    int number;
    // the row and col coordinates are for the text display
    int row;
    int col;

    bool isHorizontal;
    bool isRoad;

   public:
    Edge(int number = -1, int row = -1, int col = -1, bool isHorizontal = false);

    // get row nad column of textDisplay 2D array
    int getRow();
    int getCol();

    int getNum();
    int getOwner();
    bool getHorizontal();
    // Used to build a road at an edge. Each edge is limited
    // to one road, and costs the builder one heat and wifi resource.
    // A road can be built by a builder if an adjacent road or
    // residence has been built by the same builder.
    bool buildRoad(shared_ptr<Builder> builder, bool checkResource = true);

    // resets owner to -1 and improvement to ""
    void reset();

    // returns whether the edge has valid number, row, and col
    bool realEdge();
};

#endif
