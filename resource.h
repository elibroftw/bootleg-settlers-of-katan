#ifndef __RESOURCE__
#define __RESOURCE__
#include <iostream>
#include <string>

using std::istream;
using std::string;

enum Resource { Brick, Energy, Glass, Heat, Wifi, Park };

istream& operator>>(istream& in, Resource& x);

string getResourceName(int resourceCode);

#endif
