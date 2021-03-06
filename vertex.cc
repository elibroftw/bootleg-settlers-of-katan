#include "vertex.h"

#include <iostream>
#include <string>

using std::cerr;
using std::endl;
using std::make_pair;

Vertex::Vertex(const int number) : number{number} {
    improvement = ' ';
    if (number == -1) {
        row = -1;
        col = -1;
    } else {
        // set rowTD based on vertex number
        if (number == 0 || number == 1) {
            row = 0;
        } else if (number == 52 || number == 53) {
            row = 40;
        } else {
            row = number / 6 * 4 + 4;
        }

        // set colTD based on vertex number
        if (number == 2 || number == 48) {
            col = 11;
        } else if (number == 0 || number == 3 || number == 49 || number == 52) {
            col = 21;
        } else if (number == 1 || number == 4 || number == 50 || number == 53) {
            col = 31;
        } else if (number == 5 || number == 51) {
            col = 41;
        } else {  // i >=6 && i <= 47
            col = (number % 6) * 10 + 1;
        }
    }
}

Vertex::Vertex(const Vertex &v) : number{v.number},
                                  owner{v.owner},
                                  row{v.row},
                                  col{v.col},
                                  improvement{v.improvement} {}

Vertex::Vertex(const Vertex &&v) : number{v.number},
                                   owner{v.owner},
                                   row{v.row},
                                   col{v.col},
                                   improvement{v.improvement} {}

void Vertex::vertex_swap(Vertex &v) {
    std::swap(number, v.number);
    std::swap(owner, v.owner);
    std::swap(row, v.row);
    std::swap(col, v.col);
    std::swap(improvement, v.improvement);
}

Vertex &Vertex::operator=(const Vertex &v) {
    Vertex temp{v};
    vertex_swap(temp);
    return *this;
}

Vertex &Vertex::operator=(Vertex &&other) {
    vertex_swap(other);
    return *this;
}

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

bool Vertex::canUpgrade(const shared_ptr<Builder> &builder) {
    return improvement != 'T' && builder.get()->getNum() == owner;
}

bool Vertex::upgradeResidence(shared_ptr<Builder> &builder, const bool useResouces) {
    if (owner != -1 && owner != builder.get()->getNum()) {
        return false;
    }

    if (!useResouces && (owner == -1 || owner == builder.get()->getNum())) {
        switch (improvement) {
            case ' ':
                improvement = 'B';
                break;
            case 'B':
                improvement = 'H';
                break;
            case 'H':
                improvement = 'T';
                break;
            default:
                return false;
        }
        owner = builder.get()->getNum();
        builder.get()->addBuildingPoints();
        return true;
    }
    int numBrick = builder->getResource(Brick);
    int numEnergy = builder->getResource(Energy);
    int numGlass = builder->getResource(Glass);
    int numHeat = builder->getResource(Heat);
    int numWifi = builder->getResource(Wifi);

    switch (improvement) {
        case ' ':
            if (numBrick >= 1 && numEnergy >= 1 && numGlass >= 1 && numWifi >= 1) {
                builder->addBuildingPoints();
                builder->setResource(Brick, numBrick - 1);
                builder->setResource(Energy, numEnergy - 1);
                builder->setResource(Glass, numGlass - 1);
                builder->setResource(Wifi, numWifi - 1);
                improvement = 'B';
                owner = builder.get()->getNum();
                return true;
            }
            return false;
        case 'B':
            if (numGlass >= 2 && numHeat >= 3) {
                builder->addBuildingPoints();
                builder->setResource(Glass, numGlass - 2);
                builder->setResource(Heat, numHeat - 1);
                improvement = 'H';
                return true;
            }
            return false;
        case 'H':
            if (numBrick >= 3 && numEnergy >= 2 && numGlass >= 2 && numWifi >= 1 && numHeat >= 2) {
                builder->addBuildingPoints();
                builder->setResource(Brick, numBrick - 3);
                builder->setResource(Energy, numEnergy - 2);
                builder->setResource(Glass, numGlass - 2);
                builder->setResource(Wifi, numWifi - 2);
                builder->setResource(Heat, numHeat - 2);
                improvement = 'T';
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

pair<int, int> Vertex::getVertexFromCoords(const int row, const int col) {
    if (row % 4 != 0 || (col - 1) % 10 != 0) {
        // DEBUG STATEMENT; SHOULD NEVER BE RUN
        cerr << "something went wrong in getVertexFromCoords "
             << (row % 4 != 0) << ' ' << ((col - 1) % 10 != 0) << ' '
             << "row = " << row << ", col = " << col << endl;
    }
    return make_pair(row / 4, col / 10);
}
