#include "builder.h"

#include <chrono>
#include <ctime>
#include <ios>
#include <iostream>

#include "resource.h"

using std::cout;
using std::default_random_engine;
using std::endl;
using std::shared_ptr;
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

Builder::Builder(const string colour, const int num) : colour{colour}, num{num} {}

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

// int Builder::getResource(const int resourceCode) {
//     return resources[resourceCode];
// }

int Builder::getResource(const Resource resource) {
    return resources[resource];
}

void Builder::setResource(const int resourceCode, const int num) {
    resources[resourceCode] = num;
}

void Builder::setResource(const Resource resource, const int num) {
    resources[resource] = num;
}

int Builder::getBuildingPoints() {
    return buildPoints;
}

void Builder::setBuildingPoints(const int value) {
    buildPoints = value;
}

void Builder::addBuildingPoints(const int points) {
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

void Builder::geeseAttack(default_random_engine& rng) {
    int totalResources = 0;
    int resLost = 0;

    for (auto&& r : resources) totalResources += r;

    if (totalResources >= 10) resLost = totalResources / 2;

    if (resLost) {
        cout << "Builder " << colour << " loses " << resLost << " to the geese. They lose:" << endl;
    }
    vector<int> resourcesLost{0, 0, 0, 0, 0};
    while (resLost) {
        int randomIndex;
        for (size_t i = 0; i < 100; i++) randomIndex = resourceDistribution(rng);

        if (resources[randomIndex] != 0) {
            --resources[randomIndex];
            --resLost;
            ++resourcesLost[randomIndex];
        }
    }
    for (size_t r = 0; r < resources.size(); r++) {
        if (resourcesLost[r]) {
            cout << resourcesLost[r] << ' ' << getResourceName(static_cast<Resource>(r)) << endl;
        }
    }
}

void Builder::stealFrom(default_random_engine& rng, shared_ptr<Builder>& builderToStealFrom) {
    unsigned totalResources = 0;
    for (auto&& r : builderToStealFrom->resources) totalResources += r;
    if (!totalResources) totalResources = 1;

    unsigned randomIndex = resourceDistribution(rng);
    unsigned randResource = builderToStealFrom->resources.at(randomIndex);

    uniform_int_distribution<unsigned> prob{1, totalResources};
    unsigned val = prob(rng);
    // steal only if val is <= randResource. Allows for 100% probabilities
    if (val <= randResource) {
        --builderToStealFrom.get()->resources.at(randomIndex);
        ++resources.at(randomIndex);
        cout << "Builder " << colour << " steals " << getResourceName(static_cast<Resource>(randomIndex))
             << " from " << builderToStealFrom.get()->colour << endl;
    } else {
        cout << "Builder " << colour << " got caught slacking and could not steal from "
             << builderToStealFrom.get()->colour << "." << endl;
    }
}

void Builder::reset() {
    buildPoints = 0;
    diceIsLoaded = true;
    for (size_t i = 0; i < resources.size(); i++) resources[i] = 0;
}

void Builder::printStatus() {
    cout << "Builder " << colour << " ";

    // print padding. longest colour string is "yellow"
    for (size_t j = 0; j < (6 - colour.size()); j++) cout << " ";

    cout << "has " << buildPoints << " building points";
    for (size_t r = 0; r < resources.size(); r += 1) cout << ", " << resources[r] << ' ' << getResourceName(static_cast<Resource>(r));
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

void Builder::marketTrade(const Resource give, const Resource take) {
    if (give == take) {
        cout << "Oh. You want to give up 2 resources? Just kidding." << endl;
    } else if (resources[give] >= 3) {
        resources[give] -= 3;
        resources[take] += 1;
        cout << "Builder " << colour << "traded away 3" << getResourceName(give)
             << " for 1 " << getResourceName(take) << "." << endl;
    } else {
        cout << "ERROR: You need at least 3 " << getResourceName(give) << endl;
    }
}
