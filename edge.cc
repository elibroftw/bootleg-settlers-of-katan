#include "edge.h"

#include <iostream>
#include <string>

using std::cerr;
using std::endl;
using std::make_pair;
using std::pair;

Edge::Edge(int number) : number{number} {
    isRoad = false;
    owner = -1;
    if (number == -1) {
        row = -1;
        col = -1;
        isHorizontal = true;
    } else {
        // set colTD based on number
        if (number == 9 || number == 26 || number == 43 || number == 60) {
            col = 6;
            isHorizontal = true;
        } else if (number == 3 || number == 18 || number == 35 || number == 52 || number == 67) {
            col = 16;
            isHorizontal = true;
        } else if (number == 0 || number == 10 || number == 27 || number == 44 || number == 61 || number == 71) {
            col = 26;
            isHorizontal = true;
        } else if (number == 4 || number == 19 || number == 36 || number == 53 || number == 68) {
            col = 36;
            isHorizontal = true;
        } else if (number == 11 || number == 28 || number == 45 || number == 62) {
            col = 46;
            isHorizontal = true;
        } else if (number == 12 || number == 20 || number == 29 || number == 37 || number == 46 || number == 54) {
            col = 1;
        } else if (number == 5 || number == 13 || number == 21 || number == 30 || number == 38 || number == 47 ||
                   number == 55 || number == 63) {
            col = 11;
        } else if (number == 1 || number == 6 || number == 14 || number == 22 || number == 31 || number == 39 ||
                   number == 48 || number == 56 ||
                   number == 64 || number == 69) {
            col = 21;
        } else if (number == 2 || number == 7 || number == 15 || number == 23 || number == 32 || number == 40 ||
                   number == 49 || number == 57 ||
                   number == 65 || number == 70) {
            col = 31;
        } else if (number == 8 || number == 16 || number == 24 || number == 24 || number == 33 || number == 41 || number == 50 || number == 58 || number == 66) {
            col = 41;
        } else if (number == 17 || number == 25 || number == 34 || number == 42 || number == 51 || number == 59) {
            col = 51;
        } else {
            cerr << "SOMETHING WENT WRONG WITH SETTING COL OF EDGE #" << number << endl;
        }

        // set rowTD based on edge number
        if (number == 0) {
            row = 0;
        } else if (number == 1 || number == 2) {
            row = 2;
        } else if (number == 3 || number == 4) {
            row = 4;
        } else if (number >= 5 && number <= 8) {
            row = 6;
        } else if (number >= 6 && number <= 11) {
            row = 8;
        } else if (number >= 12 && number <= 17) {
            row = 10;
        } else if (number >= 18 && number <= 19) {
            row = 12;
        } else if (number >= 20 && number <= 25) {
            row = 14;
        } else if (number >= 26 && number <= 28) {
            row = 16;
        } else if (number >= 29 && number <= 34) {
            row = 18;
        } else if (number >= 25 && number <= 36) {
            row = 20;
        } else if (number >= 37 && number <= 42) {
            row = 22;
        } else if (number >= 43 && number <= 45) {
            row = 24;
        } else if (number >= 46 && number <= 51) {
            row = 26;
        } else if (number >= 52 && number <= 53) {
            row = 28;
        } else if (number >= 54 && number <= 59) {
            row = 30;
        } else if (number >= 60 && number <= 62) {
            row = 32;
        } else if (number >= 63 && number <= 66) {
            row = 34;
        } else if (number >= 67 && number <= 68) {
            row = 36;
        } else if (number == 69 || number == 70) {
            row = 38;
        } else if (number == 71) {
            row = 40;
        } else {
            cerr << "SOMETHING WENT WRONG SETTING ROW OF EDGE #" << number << endl;
        }
    }
}

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

pair<int, int> Edge::getEdgeFromCoords(int row, int col) {
    // 12 16

    if (row % 2 != 0 || (col - 1) % 5 != 0) {
        // DEBUG STATEMENT; SHOULD NEVER BE RUN
        cerr << "something went wrong in getEdgeFromCoords "
             << (row % 2 != 0) << ' ' << ((col - 1) % 5 != 0) << endl;
    }
    // cerr << "get edge from coords, row = " << row << ", " << col << ' ' << endl;
    return make_pair(row / 2, col / 5);
}
