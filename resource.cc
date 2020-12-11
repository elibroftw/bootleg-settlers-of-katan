#include "resource.h"

#include <algorithm>
#include <exception>
#include <ios>
#include <iostream>
#include <string>

using std::cerr;
using std::endl;
using std::istream;
using std::string;
using std::tolower;

istream& operator>>(istream& in, Resource& x) {
    string val;
    if (in >> val) {
        Resource temp = getResourceCode(val);
        if (temp == Park) {
            in.setstate(std::ios::failbit);
        } else {
            x = temp;
        }
    }
    return in;
}

Resource getResourceCode(string resourceName) {
    toupper(resourceName[0]);
    char r = resourceName[0];
    switch (r) {
        case 'B':
            return Brick;
        case 'E':
            return Energy;
        case 'G':
            return Glass;
        case 'H':
            return Heat;
        case 'W':
            return Wifi;
        default:
            return Park;
    }
    return Park;
}

string getResourceName(int resourceCode) {
    switch (resourceCode) {
        case 0:
            return "BRICK";
        case 1:
            return "ENERGY";
        case 2:
            return "GLASS";
        case 3:
            return "HEAT";
        case 4:
            return "WIFI";
        case 5:
            return "PARK";
        default:
            cerr << "ERROR getting resource name " << resourceCode << endl;
            return "PARK";
    }
}
