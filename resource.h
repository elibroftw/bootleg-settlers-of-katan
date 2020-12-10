#ifndef __RESOURCE__
#define __RESOURCE__
#include <iostream>
#include <string>

using std::istream;
using std::string;

enum Resource { Brick, Energy, Glass, Heat, Wifi, Park };

// reads a string and returns the corresponding Resource
// if string is invalid, set fail bit
istream& operator>>(istream& in, Resource& x);

// capitalize resourceName and return the
//  corresponding resource code
//  default is Park
Resource getResourceCode(string resourceName);

string getResourceName(int resourceCode);

#endif
