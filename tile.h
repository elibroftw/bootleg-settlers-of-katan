#ifndef __TILE__
#define __TILE__
#include <string>

using std::string;

class Tile {
    unsigned number;
    unsigned value;
    unsigned resource;
    bool geese;

   public:
    Tile(unsigned number, unsigned value, unsigned resourceCode);
    unsigned getNumber();

    unsigned getResource();
    void setResource(unsigned resourceCode);

    unsigned getValue();
    void setValue(unsigned resourceCode);
    void removeGeese();
    void setGeese();
};

#endif
