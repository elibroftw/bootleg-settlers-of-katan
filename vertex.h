#ifndef __VERTEX__
#define __VERTEX__
#include <memory>

#include "builder.h"

using std::shared_ptr;

class Vertex {
    int owner;
    int number;


    // the row and col will be used for text display
    // left side of vertex number starts at col
    // e.g. a number like 12 would overflow
    // but a number like 6 would become " 6"
    int row;
    int col;

    // improvement can either be "" for nothing,
    // "B" for basement, "H" for house, or "T" for tower
    char improvement;

   public:
    // default ctor for when vector is being resized
    Vertex(int number = -1, int row = -1, int col = -1);

    int getCol();
    int getRow();
    int getNum();
    int getOwner();
    char getImprovement();
    // If the current residence is a basement, can upgrade to a house.
    // If the current residence is a house, can upgrade to a tower.
    // Otherwise cannot upgrade the residence (must at least be a basement
    // and at most a tower).
    // Increases building point by one each time it is called.
    bool upgradeResidence(shared_ptr<Builder> builder, bool checkResources = true);

    // resets owner to -1 and improvement to ""
    void reset();

    // returns whether the vertex has valid number, row, and col
    bool realVertex();

    int getBuildingPoints();
};

#endif
