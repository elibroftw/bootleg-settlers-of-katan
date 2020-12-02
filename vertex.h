#ifndef __VERTEX__
#define __VERTEX__
#include <string>

class Vertex {
    int owner;
    int number;
    // the x and y coordinates will be used for text display
    int xCoord;
    int yCoord;
    std::string improvement;
    public:
        Vertex(int number, int xCoord, int yCoord);
        // turns a vertex into a basement,
        // gives the owner one building point.
        void addBasement();
        // If the current residence is a basement, upgrades to a house.
        // If the current residence is a house, upgrades to a tower.
        // Otherwise cannot upgrade the residence (must at least be a basement
        // and at most a tower).
        // Increases building point by one each time it is called.
        void upgradeResidence();

        // resets owner to -1 and improvement to ""
        void reset();
};

#endif
