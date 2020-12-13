#ifndef __BUILDER_H__
#define __BUILDER_H__
#include <memory>
#include <random>
#include <string>
#include <vector>
#include "resource.h"

using std::istream;
using std::ostream;
using std::string;
using std::uniform_int_distribution;
using std::vector;
using std::shared_ptr;

enum Colour { Blue,
              Red,
              Orange,
              Yellow };

// read string and returns the corresponding colour
// if string is invalid, set fail bit
istream& operator>>(istream& in, Colour& colour);

class Builder {
    // blue, red, orange, yellow
    string colour;
    // the turn order of the builder
    int num;
    // store number of points the builder has
    int buildPoints = 0;
    // whether the builder is using a loaded dice
    bool diceIsLoaded = true;

    // store the resources the builder has: brick, energy, glass,
    // heat, wifi
    vector<int> resources{0, 0, 0, 0, 0};
    uniform_int_distribution<unsigned> resourceDistribution{0, 4};

   public:
    Builder(const string colour, const int num);
    // When a seven is rolled, current builder moves the geese
    // to any tile. Any builder with 10 or more resources will
    // automatically lose half their resources. Resources lost
    // are chosen randomly.
    void geeseAttack();

    // the current builder (who moved the geese), can steal
    // a random resources from one builder who has a residence
    // on the tile to where the geese are moved.
    // takes care of any IO operations
    void stealFrom(shared_ptr<Builder> builderToStealFrom);

    // check if builder won the game,
    //  occurs when they have 10 points
    bool hasWon();

    // resets builder resources, buildPoints to 0
    //  and clears vertices and edges
    void reset();

    // getters

    string getColour();
    int getNum();

    // returns the number of resource of the specified int or Resource
    int getResource(const int resourceCode);
    int getResource(const Resource resource);

    void setResource(const Resource resource, const int num);
    void setResource(const int resourceCode, const int num);

    int getBuildingPoints();
    void setBuildingPoints(const int value);
    // increments the builder's building points
    void addBuildingPoints(int points = 1);

    // dice settings
    void useLoadedDice();
    void useFairDice();
    bool isDiceLoaded();

    // trade 3 give for 1 take
    void marketTrade(const Resource give, const  Resource take);

    // prints out resources space separated (ends with a space) e.g. 0 0 3 0 0
    void printResources(ostream& out);

    // prints out status for the builder
    void printStatus();

    // prints out win message for the builder. Includes fireworks.
    void winGame();
};

#endif
