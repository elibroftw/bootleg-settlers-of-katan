#include "builder.h"

#include <chrono>
#include <ctime>
#include <ios>
#include <iostream>

#include "resource.h"

using std::cout;
using std::default_random_engine;
using std::endl;
using std::toupper;

// read string, use only the capitalized first letter to determine the builder
// if fail, in.setstate(std::ios::failbit);
istream& operator>>(istream& in, Colour& colour) {
    string temp;
    if (in >> temp) {
        char c = toupper(temp[0]);
        switch (c) {
            case 'B':
            case '0':
                colour = Blue;
                break;
            case 'R':
            case '1':
                colour = Red;
                break;
            case 'O':  // this is an O not a zero
            case '2':
                colour = Orange;
                break;
            case 'Y':
            case '3':
                colour = Yellow;
                break;
            default:
                in.setstate(std::ios::failbit);
        }
    }
    return in;
}

Builder::Builder(string colour, int num) : colour{colour}, num{num} {}

bool Builder::hasWon() {
    return buildPoints >= 10;
}

string Builder::getColour() {
    return colour;
}

int Builder::getNum() { return num; }

void Builder::printResources(ostream& out) {
    for (auto&& r : resources) {
        out << r << " ";
    }
}

int Builder::getResource(int resourceCode) {
    return resources[resourceCode];
}

int Builder::getResource(string resourceName) {
    return resources[getResourceCode(resourceName)];
}

void Builder::setResource(int resourceCode, int num) {
    resources[resourceCode] = num;
}

void Builder::setResource(string resourceName, int num) {
    resources[getResourceCode(resourceName)] = num;
}

int Builder::getBuildingPoints() {
    return buildPoints;
}

void Builder::setBuildingPoints(int value) {
    buildPoints = value;
}

void Builder::addBuildingPoints(int points) {
    buildPoints += points;
}

void Builder::useLoadedDice() {
    diceIsLoaded = true;
}

void Builder::useFairDice() {
    diceIsLoaded = false;
}

bool Builder::isDiceLoaded() {
    return diceIsLoaded;
}

void Builder::geeseAttack() {
    int totalResources = 0;
    int resourceToLose = 0;

    for (auto&& r : resources) totalResources += r;

    if (totalResources >= 10) resourceToLose = totalResources / 2;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng{seed};

    while (resourceToLose) {
        int randomIndex = resourceDistribution(rng);
        if (resources[randomIndex] != 0) {
            --resources[randomIndex];
            --resourceToLose;
        }
    }
}

int Builder::stealFrom(std::shared_ptr<Builder> builder) {
    int totalResources = 0;
    for (auto&& r : builder->resources) totalResources += r;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng{seed};
    int randomIndex = resourceDistribution(rng);
    int randResource = builder->resources[randomIndex];
    int stealProb = randResource / totalResources; 
    
}

void Builder::reset() {
    buildPoints = 0;
    diceIsLoaded = true;
    for (size_t i = 0; i < resources.size(); i++) {
        resources[i] = 0;
    }
}

void Builder::printStatus() {
    unsigned padding = 6 - colour.size();
    cout << "Builder " << colour << " ";
    for (size_t j = 0; j < padding; j++) {
        cout << " ";
    }
    cout << "has " << buildPoints << " building points";
    for (size_t r = 0; r < resources.size(); r++) {
        cout << ", " << resources[r] << ' ' << getResourceName(r);
    }
    cout << endl;
}

void Builder::winGame() {
    cout << "Builder " << colour << " has won the game!!!" << endl;
    // Art by Joan Stark
    cout << R"(                                   .''.)" << endl;
    cout << R"(       .''.      .        *''*    :_\/_:     .)" << endl;
    cout << R"(      :_\/_:   _\(/_  .:.*_\/_*   : /\ :  .'.:.'.)" << endl;
    cout << R"(  .''.: /\ :   ./)\   ':'* /\ * :  '..'.  -=:o:=-)" << endl;
    cout << R"( :_\/_:'.:::.    ' *''*    * '.\'/.' _\(/_'.':'.')" << endl;
    cout << R"( : /\ : :::::     *_\/_*     -= o =-  /)\    '  *)" << endl;
    cout << R"(  '..'  ':::'     * /\ *     .'/.\'.   ')" << endl;
    cout << R"(      *            *..*         :)" << endl;
    cout << R"(        *)" << endl;
    cout << R"(        *)" << endl;
}
