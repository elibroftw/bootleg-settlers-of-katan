#ifndef __BUILDER__
#define __BUILDER__
#include <string>
#include <vector>
#include <iostream>


using std::string;
using std::ostream;
using std::vector;
using std::istream;

enum Colour { Blue, Red, Orange, Yellow };

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
    vector<int> resources {0, 0, 0, 0, 0};

    public:
        Builder(string colour, int num);
        // When a seven is rolled, current builder moves the geese
        // to any tile. Any builder with 10 or more resources will
        // automatically lose half their resources. Resources lost
        // are chosen randomly.
        void geeseAttack();
        // the current builder (who moved the geese), can steal
        // a random resources from one builder who has a residence
        // on the tile to where the geese are moved.
        // if not negative, steal was sucessful...
        int tryStealing();
        // check if builder won the game, which occurs when they
        // have 10 points
        bool hasWon();

        // resets builder resources, buildPoints to 0
        //  and clears vertices and edges
        void reset();

        string getColour();
        int getNum();

        // returns the number of resource of the specified string or int
        // if string, first toLower the string
        int getResource(std::string resourceName);
        int getResource(int resourceCode);

        // prints out resources space separated (ends with a space) e.g. 0 0 3 0 0
        void printResources(ostream &out);

        void setResource(std::string resourceName, int num);
        void setResource(int resourceCode, int num);

        int getBuildingPoints();
        void setBuildingPoints(int value);

        // increments the builder's building points
        void addBuildingPoints(int points = 1);

        void useLoadedDice();
        void useFairDice();
        bool isDiceLoaded();
};

#endif
