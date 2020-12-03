#include "textdisplay.h"

#include <string>
#include <vector>

using std::make_pair;
using std::ostream;
using std::pair;
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

    // add pipes (|) and dashes (-)
    for (size_t i = 0; i < 10; i++) {
        int top = i * 4;
        for (size_t j = 0; j < 5; j++) {
            // set vertex pipes given conditions
            if ((i >= 2 && i <= 8) || j == 2 || j == 3 || ((i == 1 || i == 9) && (j == 1 || j == 4))) {
                board[top][j * 10] = '|';
                board[top][j * 10 + 3] = '|';
                // if at the top left of a tile
                if (j % 2 == i % 2) {
                    board[top][j * 10 + 4] = '-';
                    board[top][j * 10 + 5] = '-';
                }
                // add bottom pipes
                if (top != 40) {
                    board[top + 1][j * 10] = '|';
                    board[top + 3][j * 10] = '|';
                }
            }
        }
    }

    // add tile numbers
    for (size_t i = 0; i < 19; i++) {
        setTileNumber(i);
    }

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

pair<int, int> TextDisplay::getTopLeftCoord(int tileNumber) {
    // returns the location of the top left "|" of the tileNumber
    int xCoord;
    int yCoord;
    if (tileNumber == 0) {
        xCoord = 0;
    } else if (tileNumber == 1 || tileNumber == 2) {
        xCoord = 4;
    } else if (tileNumber >= 3 && tileNumber <= 5) {
        xCoord = 8;
    } else if (tileNumber == 6 || tileNumber == 7) {
        xCoord = 12;
    } else if (tileNumber >= 8 && tileNumber <= 10) {
        xCoord = 16;
    } else if (tileNumber == 11 || tileNumber == 12) {
        xCoord = 20;
    } else if (tileNumber >= 13 && tileNumber <= 15) {
        xCoord = 24;
    } else if (tileNumber == 16 || tileNumber <= 17) {
        xCoord = 28;
    } else {  //  tileNumber == 18
        xCoord = 32;
    }
    // yCoord
    if (tileNumber == 0 || tileNumber == 4 || tileNumber == 9 || tileNumber == 14 || tileNumber == 18) {
        yCoord = 20;
    } else if (tileNumber == 1 || tileNumber == 6 || tileNumber == 11 || tileNumber == 16) {
        yCoord = 10;
    } else if (tileNumber == 2 || tileNumber == 7 || tileNumber == 12 || tileNumber == 17) {
        yCoord = 30;
    } else if (tileNumber == 3 || tileNumber == 8 || tileNumber == 13) {
        yCoord = 0;
    } else {  // tileNumber == 5 || tileNumber == 10 || tileNumber == 15
        yCoord = 40;
    }

    return make_pair(xCoord, yCoord);
}

void TextDisplay::setResourceName(int tileNumber, string resource) {
    auto tileCoords = getTopLeftCoord(tileNumber);
    int xCoord = tileCoords.first + 3;
    int yCoord = tileCoords.second + 2;
    for (size_t i = 0; i < resource.size(); i++) {
        board[xCoord][yCoord + i] = resource[i];
    }
}


void TextDisplay::setTileNumber(int tileNumber) {
    auto tileCoords = getTopLeftCoord(tileNumber);
    int xCoord = tileCoords.first + 2;
    int yCoord = tileCoords.second + 4;
    setInt(xCoord, yCoord, tileNumber);
}

void TextDisplay::setTileValue(int tileNumber, int tileValue) {
    auto tileCoords = getTopLeftCoord(tileNumber);
    int xCoord = tileCoords.first + 4;
    int yCoord = tileCoords.second + 4;
    setInt(xCoord, yCoord, tileValue);
}

void TextDisplay::setGoose(int tileNumber) {
    auto tileCoords = getTopLeftCoord(tileNumber);
    int xCoord = tileCoords.first + 3;
    int yCoord = tileCoords.second + 3;
    string geese = "GEESE";
    for (size_t i = 0; i < geese.size(); i++) {
        board[xCoord][yCoord + i] = geese[i];
    }
}

void TextDisplay::removeGoose(int tileNumber) {
    auto tileCoords = getTopLeftCoord(tileNumber);
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
