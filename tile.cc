#include "tile.h"

#include <string>

using std::string;

Tile::Tile(unsigned number, unsigned value, unsigned resourceCode) : number{number}, value{value}, resource{resourceCode} {}

unsigned Tile::getNumber() {
    return number;
}

unsigned Tile::getResource() {
    return resource;
}

unsigned Tile::getValue() {
    return value;
}
