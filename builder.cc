#include "builder.h"

#include <ios>
#include <iostream>
#include <string>
#include <ctime>


#include "resource.h"

using std::istream;
using std::ostream;
using std::string;
using std::toupper;
using std::vector;

// read string, use only the capitalized first letter to determine the builder
// if fail, in.setstate(std::ios::failbit);
istream& operator>>(istream& in, Colour& colour) {
    string temp;
    if (in >> temp) {
        char c = toupper(temp[0]);
        switch (c) {
            case 'B':
                colour = Blue;
                break;
            case 'R':
                colour = Red;
                break;
            case 'Y':
                colour = Yellow;
                break;
            case 'O':
                colour = Orange;
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
    int numResources = 0;
    int resourcesLost = 0;
    for(std::vector<int>::iterator it = resources.begin(); it != resources.end(); ++it) {
        numResources += *it;
    }
    if (numResources >= 10) {
        resourcesLost = numResources / 2;
    }
    int counter = 0;
    srand((unsigned int)time(NULL));
    while (counter < resourcesLost) {
        int randomIndex = rand() % resources.size();
        if (resources[randomIndex] != 0) {
            --resources[randomIndex];
            ++counter;
        }
    }
}

int Builder::stealFrom(std::shared_ptr<Builder> builder) {
    

}

void Builder::reset() {
    buildPoints = 0;
    diceIsLoaded = true;
    for (size_t i = 0; i < resources.size(); i++) {
        resources[i] = 0;
    }
}
