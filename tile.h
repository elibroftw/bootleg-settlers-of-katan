#ifndef __TILE_H__
#define __TILE_H__
#include <string>

using std::string;

class Tile {
    unsigned number;
    unsigned value;
    unsigned resource;

   public:
    Tile(unsigned number, unsigned value, unsigned resourceCode);
    unsigned getNumber();

    unsigned getResource();
    // void setResource(unsigned resourceCode);

    unsigned getValue();
    // void setValue(unsigned newValue);
};

#endif
