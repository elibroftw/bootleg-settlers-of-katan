#ifndef __TILE__
#define __TILE__
#include <string>
#include <map>

class Tile {
    unsigned int number;
    unsigned int value;
    resource;
    std::map<std::string, Vertex> vertices;
    bool geese;
    std::map<std::string, Edge> edges;
};

#endif
