#ifndef __BUILDER_H__
#define __BUILDER_H__
#include <string>
#include <vector>
#include <iostream>


using std::string;
using std::ostream;
using std::vector;

class Builder {
    // indicate colour: red, blue, orange, yellow
    string colour;
    // store the resources the builder has: brick, energy, glass,
    // heat, wifi
    vector<int> resources;
    // store number of points the builder has
    int buildPoints;
    public:
        Builder(string colour);
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

        // resets builder resources, buildPoints to 0
        //  and clears vertices and edges
        void reset();

        // return the number of resource of the specified string or int
        // if string, first toLower the string
        int getResource(std::string resourceName);
        int getResource(int resourceCode);

        int setResource(std::string resourceName, int num);
        int setResource(int resourceCode, int num);

        int getBuilderPoints();
        void setBuilderPoints(int value);

        friend ostream &operator<<(ostream &out, const Builder &builder);
};

ostream &operator<<(ostream &out, const Builder &builder);

#endif
