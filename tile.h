#ifndef __TILE__
#define __TILE__
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class Tile {
    unsigned int number;
    unsigned int value;
    string resource;
    unordered_map<std::string, Vertex> vertices;
    bool geese;
    unordered_map<std::string, Edge> edges;
    public:
        string getResource();
        int getNumber();
        int getValue();
        void removeGeese();
        void setGeese();
};

#endif
