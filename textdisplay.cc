#include "textdisplay.h"

#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::to_string;
using std::vector;

TextDisplay::TextDisplay() {
  // first itialize with nothing
    for (size_t i = 0; i < 41; i++) {
        vector<char> tempRow;
        for (size_t j = 0; j < 54; j++) {
            tempRow.push_back(' ');
        }
        board.push_back(tempRow);
    }
    // now set all | (pipes)
    // and then set all - (dash)
}

void TextDisplay::setChar(int x, int y, char c) {
    // assumes x >= 0 and y >= 0;
    board[x][y] = c;
}

void TextDisplay::setString(int x, int y, string s) {
    for (size_t i = 0; i < s.size(); i++, x++) {
        board[x][y] = s[i];
    }
}

void TextDisplay::setInt(int x, int y, int v) {
    // assumes x >= 0 and y >= 0;
    string int_as_str;
    if (v < 10) {
        int_as_str = " " + to_string(v);
    } else {
        int_as_str = to_string(v);
    }
    // for char in string, set it to board starting from (x, y)
    for (size_t i = 0; i < int_as_str.size(); i++, x++) {
        board[x][y] = int_as_str[i];
    }
}

ostream &operator<<(ostream &out, const TextDisplay &td) {
    for (auto &&row : td.board) {
        for (auto &&c : row) {
            out << c;
        }
        out << std::endl;
    }
    return out;
}
