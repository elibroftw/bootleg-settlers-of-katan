#ifndef __TILE_H__
#define __TILE_H__
#include "resource.h"

class Tile {
    unsigned number;
    unsigned value;
    Resource resource;

   public:
    Tile(const unsigned number, const unsigned value, const Resource resourceCode);

    // getters

    unsigned getNumber();

    unsigned getValue();

    Resource getResource();
};

#endif
