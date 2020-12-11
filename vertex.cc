#include "vertex.h"

#include <string>

Vertex::Vertex(int number, int row, int col) : number{number}, row{row}, col{col}, improvement{' '} {}

int Vertex::getCol() {
    return col;
}

int Vertex::getRow() {
    return row;
}

int Vertex::getNum() {
    return number;
}

int Vertex::getOwner() {
    return owner;
}

char Vertex::getImprovement() {
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

bool Vertex::canUpgrade(shared_ptr<Builder> builder) {
    return improvement != 'T' && builder.get()->getNum() == owner;
}

bool Vertex::upgradeResidence(shared_ptr<Builder> builder, bool useResouces) {
    if (!useResouces && (owner == -1 || owner == builder.get()->getNum())) {
        owner = builder.get()->getNum();
        switch (improvement) {
            case ' ':
                improvement = 'B';
                return true;
            case 'B':
                improvement = 'H';
                return true;
            case 'T':
                improvement = 'T';
                return true;
            default:
                return false;
        }
    }
    int numBrick = builder->getResource("brick");
    int numEnergy = builder->getResource("energy");
    int numGlass = builder->getResource("glass");
    int numHeat = builder->getResource("heat");
    int numWifi = builder->getResource("wifi");

    switch (improvement) {
        case ' ':
            if (numBrick >= 1 && numEnergy >= 1 && numGlass >= 1 && numWifi >= 1) {
                builder->addBuildingPoints();
                builder->setResource("brick", numBrick - 1);
                builder->setResource("energy", numEnergy - 1);
                builder->setResource("glass", numGlass - 1);
                builder->setResource("wifi", numWifi - 1);
                improvement = 'B';
                return true;
            }
            return false;
        case 'B':
            if (numGlass >= 2 && numHeat >= 3) {
                builder->addBuildingPoints();
                builder->setResource("glass", numGlass - 2);
                builder->setResource("heat", numHeat - 1);
                return true;
            }
            return false;
        case 'H':
            if (numBrick >= 3 && numEnergy >= 2 && numGlass >= 2 && numWifi >= 1 && numHeat >= 2) {
                builder->addBuildingPoints();
                builder->setResource("brick", numBrick - 3);
                builder->setResource("energy", numEnergy - 2);
                builder->setResource("glass", numGlass - 2);
                builder->setResource("wifi", numWifi - 2);
                builder->setResource("heat", numHeat - 2);
                return true;
            }
            return false;
    }
    // improvement is a tower
    return false;
}

void Vertex::reset() {
    owner = -1;
    improvement = ' ';
}

bool Vertex::realVertex() {
    if (getCol() >= 0 && getRow() >= 0 && getNum() >= 0) {
        return true;
    }
    return false;
}
