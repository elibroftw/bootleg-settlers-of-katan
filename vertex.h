#ifndef __VERTEX__
#define __VERTEX__
#include <string>
#include "builder.h"

class Vertex {
    int owner;
    int number;
    // the x and y coordinates will be used for text display
    // left side of vertex number starts at xCoord
    // e.g. a number like 12 would overflow
    // but a number like 6 would become " 6"
    int xCoord;
    // yCoord is the "row" of the textDisplay
    int yCoord;
    std::string improvement;

   public:
    // default ctor for when vector is being resized
    Vertex(int number = -1, int xCoord = -1, int yCoord = -1);

    int getX();
    int getY();
    int getNum();
    int getOwner();
    // turns a vertex into a basement,
    // gives the owner one building point.
    void addBasement(shared_ptr<Builder> builder, bool checkResources=true);
    // If the current residence is a basement, upgrades to a house.
    // If the current residence is a house, upgrades to a tower.
    // Otherwise cannot upgrade the residence (must at least be a basement
    // and at most a tower).
    // Increases building point by one each time it is called.
    void upgradeResidence();

    // resets owner to -1 and improvement to ""
    void reset();

    // returns whether the vertex has valid number, xCoord, and yCoord
    bool realVertex();
};

#endif
