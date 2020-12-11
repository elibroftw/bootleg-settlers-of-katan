#include "tile.h"

Tile::Tile(unsigned number, unsigned value, unsigned resourceCode) : number{number}, value{value}, resource{resourceCode} {}

unsigned Tile::getNumber() {
    return number;
}

unsigned Tile::getValue() {
    return value;
}

unsigned Tile::getResource() {
    return resource;
}
