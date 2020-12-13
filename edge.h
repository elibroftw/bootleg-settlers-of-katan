#ifndef __EDGE_H__
#define __EDGE_H__
#include <memory>

#include "builder.h"

using std::pair;
using std::shared_ptr;

class Edge {
    int number;
    int owner = -1;
    // the row and col coordinates are for the text display
    int row;
    int col;

    bool isHorizontal = false;
    bool isRoad = false;
    void edge_swap(Edge &e);

   public:
    // default ctor for when vector is being resized
    Edge(const int number = -1);
    Edge(const Edge &e);
    Edge &operator=(const Edge &e);

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
    bool buildRoad(const shared_ptr<Builder> &builder, const bool useResources = true);

    // resets owner to -1 and improvement to ""
    void reset();

    // returns whether the edge has valid number, row, and col
    bool realEdge();

    // given the row, column coordinates of the edge in the textDisplay,
    // return the row, column indices for the edges maps
    pair<int, int> static getEdgeFromCoords(const int row, const int col);
};

#endif
