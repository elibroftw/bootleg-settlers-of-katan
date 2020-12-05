#include "vertex.h"

Vertex::Vertex(int number, int xCoord, int yCoord) : number{number}, xCoord{xCoord}, yCoord{yCoord}, improvement{' '} {}

int Vertex::getX() {
    return xCoord;
}

int Vertex::getY() {
    return yCoord;
}

int Vertex::getNum() {
    return number;
}

int Vertex::getOwner() {
    return owner;
}

int Vertex::getImprovement() {
    return improvement;
}

int Vertex::getBuildingPoints() {
    switch (improvement) {
        case 'B':
            return 1;
        case 'H':
            return 2;
        case 'T':
            return 3;
        default:
            return 0;
    }
}

bool Vertex::upgradeResidence(shared_ptr<Builder> builder, bool checkResources) {
    std::string errorMsg = "You do not have enough resources.";
    int numBrick = builder->getResource("brick");
    int numEnergy = builder->getResource("energy");
    int numGlass = builder->getResource("glass");
    int numHeat = builder->getResource("heat");
    int numWifi = builder->getResource("wifi");
    if (improvement == ' ') {
        if (numBrick >= 1 && numEnergy >= 1 && numGlass >= 1 && numWifi >= 1) {
            builder->IncreasePoint();
            builder->setResource("brick", numBrick - 1);
            builder->setResource("energy", numEnergy - 1);
            builder->setResource("glass", numGlass - 1);
            builder->setResource("wifi", numWifi - 1);
            return true;
        } else {
            std::cout << errorMsg << std::endl;
            return false;
        }
    } else if (improvement == 'B') {
        if (numGlass >= 2 && numHeat >= 3) {
            builder->IncreasePoint();
            builder->setResource("glass", numGlass - 2);
            builder->setResource("heat", numHeat - 1);
            return true;
        } else {
            std::cout << errorMsg << std::endl;
            return false;
        }
    } else if (improvement == 'H') {
        if (numBrick >= 3 && numEnergy >= 2 && numGlass >= 2 && numWifi >= 1 && numHeat >= 2) {
            builder->IncreasePoint();
            builder->setResource("brick", numBrick - 3);
            builder->setResource("energy", numEnergy - 2);
            builder->setResource("glass", numGlass - 2);
            builder->setResource("wifi", numWifi - 2);
            builder->setResource("heat", numHeat - 2);
            return true;
        } else {
            std::cout << errorMsg << std::endl;
            return false;
        }
    } else {  //improvement is a tower
        return false;
    }
}

void Vertex::reset() {
    owner = -1;
    improvement = ' ';
}

bool Vertex::realVertex() {
    if (getX() >= 0 && getY() >= 0 && getNum() >= 0) {
        return true;
    }
    return false;
}
