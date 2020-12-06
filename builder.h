#ifndef __BUILDER__
#define __BUILDER__
#include <string>
#include <vector>
#include <iostream>


using std::string;
using std::ostream;
using std::vector;
using std::istream;

enum Colour { blue, red, orange, yellow };

// read string, use only the capitalized first letter to determine the builder
// if fail, in.setstate(std::ios::failbit);
istream& operator>>(istream& in, Colour& x);

class Builder {
    // indicate colour: red, blue, orange, yellow
    string colour;
    // store the resources the builder has: brick, energy, glass,
    // heat, wifi
    vector<int> resources;
    // store number of points the builder has
    int buildPoints;
    bool diceIsLoaded;
    public:
        // TODO (in builder / vertex) for building basements,
        // after verifying that the vertex does not have any adjacent buildings,
        // for each road builder has built,
        // check for (yCoordE == yCoordV && xCoordV - 5 <= xCoordE <= xCoordV + 5) OR
        // check for (xCoordE == xCoordV && yCoordV - 2 <= xCoordE <= xCoordV + 2)

        Builder(string colour);
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

        string getColour(bool firstCharOnly=false);

        // return the number of resource of the specified string or int
        // if string, first toLower the string
        int getResource(std::string resourceName);
        int getResource(int resourceCode);

        int setResource(std::string resourceName, int num);
        int setResource(int resourceCode, int num);

        int getBuilderPoints();
        void setBuilderPoints(int value);
        // increases the builder's points by one (must upgrade residence)
        void IncreasePoint();

        void useLoadedDice();
        void useFairDice();
        bool isDiceLoaded();

        friend ostream &operator<<(ostream &out, const Builder &builder);
};

// prints out resources
ostream &operator<<(ostream &out, const Builder &builder);

#endif
