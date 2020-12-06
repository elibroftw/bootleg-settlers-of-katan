#include "resource.h"

#include <algorithm>
#include <exception>
#include <ios>
#include <iostream>
#include <string>

using std::istream;
using std::string;

istream& operator>>(istream& in, Resource& x) {
    string val;
    if (in >> val) {
        for (size_t i = 0; i < val.size(); i++) {
            std::toupper(val[i]);
        }
        if (val == "BRICK") {
            x = Resource::Brick;
        } else if (val == "ENERGY") {
            x = Resource::Energy;
        } else if (val == "GLASS") {
            x = Resource::Glass;
        } else if (val == "HEAT") {
            x = Resource::Heat;
        } else if (val == "WIFI" || val == "") {
            x = Resource::Glass;
        } else {
            // x = Resource::Park;
            in.setstate(std::ios::failbit);
        }
    }

    return in;
}

string getResourceName(int resourceCode) {
    switch (resourceCode) {
        case 0:
            return "BRICKS";
        case 1:
            return "ENERGY";
        case 2:
            return "GLASS";
        case 3:
            return "HEAT";
        case 4:
            return "WIFI";
        default:
            return "PARK";
    }
}
