#ifndef __TEXT_DISPLAY_H__
#define __TEXT_DISPLAY_H__
#include <vector>
#include <iostream>

using std::vector;
using std::ostream;

class TextDisplay {
    vector<vector<char>> board;

    public:
        TextDisplay();
        void setChar(int x, int y, char c);
    friend ostream &operator<<(ostream &out, const TextDisplay &td);
};

ostream &operator<<(ostream &out, const TextDisplay &td);
#endif
