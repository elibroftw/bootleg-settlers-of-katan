#ifndef __TILE__
#define __TILE__
#include <string>

using std::string;

class Tile {
    unsigned int number;
    unsigned int value;
    unsigned int resource;
    bool geese;
    public:
        Tile(unsigned int number, unsigned int value, unsigned int resourceCode);
        unsigned int getNumber();

        unsigned int getResource();
        void setResource(unsigned int resourceCode);

        unsigned int getValue();
        void setValue(unsigned int resourceCode);
        void removeGeese();
        void setGeese();
};

#endif
