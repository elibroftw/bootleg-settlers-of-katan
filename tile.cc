#include "tile.h"

Tile::Tile(const unsigned number, const unsigned value, const Resource resourceCode) : number{number}, value{value}, resource{resourceCode} {}

unsigned Tile::getNumber() {
    return number;
}

unsigned Tile::getValue() {
    return value;
}

Resource Tile::getResource() {
    return resource;
}
