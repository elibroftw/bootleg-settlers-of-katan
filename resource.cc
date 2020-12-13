#include "resource.h"

#include <algorithm>
#include <exception>
#include <ios>

using std::cerr;
using std::endl;
using std::toupper;

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

Resource getResourceCode(const string resourceName) {
    char r = toupper(resourceName[0]);

    switch (r) {
        case 'B':
        case '0':
            return Brick;
        case 'E':
        case '1':
            return Energy;
        case 'G':
        case '2':
            return Glass;
        case 'H':
        case '3':
            return Heat;
        case 'W':
        case '4':
            return Wifi;
        default:
            return Park;
    }
    return Park;
}

string getResourceName(const Resource resourceCode) {
    switch (resourceCode) {
        case Brick:
            return "BRICK";
        case Energy:
            return "ENERGY";
        case Glass:
            return "GLASS";
        case Heat:
            return "HEAT";
        case Wifi:
            return "WIFI";
        case Park:
            return "PARK";
        default:
            cerr << "ERROR getting resource name " << resourceCode << endl;
            return "PARK";
    }
}
