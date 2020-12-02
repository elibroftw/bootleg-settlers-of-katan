#ifndef __BUILDER_H__
#define __BUILDER_H__
#include <string>
#include <vector>

class Builder {
    // indicate colour: red, blue, orange, yellow
    std::string colour;
    // store the resources the builder has: brick, energy, glass,
    // heat, wifi
    std::vector<int> resources;
    // store number of points the builder has
    int buildPoints;
    public:
        Builder(std::string colour);
        // When a seven is rolled, current builder moves the geese
        // to any tile. Any builder with 10 or more resources will
        // automatically lose half their resources. Resources lost
        // are chosen randomly.
        void geeseAttack();
        // the current builder (who moved the geese), can steal
        // a random resources from one builder who has a residence
        // on the tile to where the geese are moved.
        bool tryStealing();
        // check if builder won the game, which occurs when they
        // have 10 points
        bool hasWon();
};


#endif
