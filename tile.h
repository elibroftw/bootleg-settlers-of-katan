#ifndef __TILE_H__
#define __TILE_H__

class Tile {
    unsigned number;
    unsigned value;
    unsigned resource;

   public:
    Tile(unsigned number, unsigned value, unsigned resourceCode);

    // getters

    unsigned getNumber();

    unsigned getValue();

    unsigned getResource();
};

#endif
