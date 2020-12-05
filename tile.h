#ifndef __TILE__
#define __TILE__
#include <string>

using std::string;

class Tile {
    unsigned int number;
    unsigned int value;
    string resource;
    bool geese;
    public:
        string getResource();
        int getNumber();
        int getValue();
        void removeGeese();
        void setGeese();
};

#endif
