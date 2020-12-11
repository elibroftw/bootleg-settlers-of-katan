#include <string>

#include "edge.h"

Edge::Edge(int number, int row, int col, bool isHorizontal) :
    number{number}, row{row}, col{col}, isHorizontal{isHorizontal} {}

int Edge::getRow() {
    return row;
}

int Edge::getCol() {
    return col;
}

int Edge::getNum() {
    return number;
}

int Edge::getOwner() {
    return owner;
}

bool Edge::getHorizontal() {
    return isHorizontal;
}

// don't understand how to use checkResources here or in upgradeResidence, will go over it with you Friday
bool Edge::buildRoad(shared_ptr<Builder> builder, bool useResources) {
    std::string errorMsg = "You do not have enough resources.";
    int numHeat = builder->getResource("heat");
    int numWifi = builder->getResource("wifi");
    return numHeat >= 1 && numWifi >= 1 ? true : false;
}

void Edge::reset() {
    owner = -1;
    // not sure why we set improvement to ""
}

bool Edge::realEdge() {
    if (getCol() >= 0 && getRow() >= 0 && getNum() >= 0) {
        return true;
    }
    return false;
}
