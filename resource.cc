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

Resource getResourceCode(string resourceName) {
    char r = toupper(resourceName[0]);
    ;
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
