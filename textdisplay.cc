#include "textdisplay.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::make_pair;
using std::ostream;
using std::pair;
using std::shared_ptr;
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
    for (size_t i = 0; i < 11; i++) {
        int top = i * 4;
        for (size_t j = 0; j < 6; j++) {
            // set vertex pipes given conditions
            if ((i >= 2 && i <= 8) || j == 2 || j == 3 || ((i == 1 || i == 9) && (j == 1 || j == 4))) {
                board[top][j * 10] = '|';
                board[top][j * 10 + 3] = '|';
                // if at the top left of a tile
                if (j % 2 == i % 2) {
                    board[top][j * 10 + 4] = '-';
                    board[top][j * 10 + 5] = '-';
                    board[top][j * 10 + 8] = '-';
                    board[top][j * 10 + 9] = '-';
                }
                // add bottom pipes
                if (top != 40 && (top != 36 || j == 2 || j == 3) && (top != 32 || (j >= 1 && j <= 4))) {
                    board[top + 1][j * 10 + 2] = '|';
                    board[top + 3][j * 10 + 2] = '|';
                }
            }
        }
    }

    // add tile numbers
    for (size_t i = 0; i < 19; i++) {
        setTileNumber(i);
    }
}

void TextDisplay::setChar(int row, int col, char val) {
    // assumes row >= 0 and col >= 0;
    board[row][col] = val;
}

void TextDisplay::setString(int row, int col, string str) {
    for (size_t i = 0; i < str.size(); i++, col++) {
        board[row][col] = str[i];
    }
}

void TextDisplay::setInt(int row, int col, int num) {
    // assumes row >= 0 and col >= 0;
    string int_as_str;
    if (num < 10) {
        int_as_str = " " + to_string(num);
    } else {
        int_as_str = to_string(num);
    }
    // for char in string, set it to board starting from (x, y)
    for (size_t i = 0; i < int_as_str.size(); i++, col++) {
        board[row][col] = int_as_str[i];
    }
}

pair<int, int> TextDisplay::getTopLeftCoord(int tileNumber) {
    // returns the location of the top left "|" of the tileNumber
    int row;
    int col;
    if (tileNumber == 0) {
        row = 0;
    } else if (tileNumber == 1 || tileNumber == 2) {
        row = 4;
    } else if (tileNumber >= 3 && tileNumber <= 5) {
        row = 8;
    } else if (tileNumber == 6 || tileNumber == 7) {
        row = 12;
    } else if (tileNumber >= 8 && tileNumber <= 10) {
        row = 16;
    } else if (tileNumber == 11 || tileNumber == 12) {
        row = 20;
    } else if (tileNumber >= 13 && tileNumber <= 15) {
        row = 24;
    } else if (tileNumber == 16 || tileNumber <= 17) {
        row = 28;
    } else {  //  tileNumber == 18
        row = 32;
    }
    // yCoord
    if (tileNumber == 0 || tileNumber == 4 || tileNumber == 9 || tileNumber == 14 || tileNumber == 18) {
        col = 20;
    } else if (tileNumber == 1 || tileNumber == 6 || tileNumber == 11 || tileNumber == 16) {
        col = 10;
    } else if (tileNumber == 2 || tileNumber == 7 || tileNumber == 12 || tileNumber == 17) {
        col = 30;
    } else if (tileNumber == 3 || tileNumber == 8 || tileNumber == 13) {
        col = 0;
    } else {  // tileNumber == 5 || tileNumber == 10 || tileNumber == 15
        col = 40;
    }

    return make_pair(row, col);
}

void TextDisplay::setTileResource(int tileNumber, string resource) {
    auto tileCoords = getTopLeftCoord(tileNumber);
    int row = tileCoords.first + 3;
    int col = tileCoords.second + 2;
    for (size_t i = 0; i < resource.size(); i++) {
        board[row][col + i] = resource[i];
    }
}

void TextDisplay::setTileNumber(int tileNumber) {
    auto tileCoords = getTopLeftCoord(tileNumber);
    int row = tileCoords.first + 2;
    int col = tileCoords.second + 6;
    setInt(row, col, tileNumber);
}

void TextDisplay::setTileValue(int tileNumber, int tileValue) {
    if (tileValue != 7) {
        auto tileCoords = getTopLeftCoord(tileNumber);
        int row = tileCoords.first + 4;
        int col = tileCoords.second + 4;
        setInt(row, col, tileValue);
    }
}

void TextDisplay::setGeese(int tileNumber) {
    auto tileCoords = getTopLeftCoord(tileNumber);
    int row = tileCoords.first + 3;
    int col = tileCoords.second + 3;
    string geese = "GEESE";
    for (size_t i = 0; i < geese.size(); i++) {
        board[row][col + i] = geese[i];
    }
}

void TextDisplay::removeGeese(int tileNumber) {
    auto tileCoords = getTopLeftCoord(tileNumber);
    int row = tileCoords.first + 3;
    int col = tileCoords.second + 3;
    string geese = "GEESE";
    for (size_t i = 0; i < geese.size(); i++) {
        board[row][col + i] = ' ';
    }
}

void TextDisplay::updateVertex(shared_ptr<Vertex> &vertex, shared_ptr<Builder> &builder) {
    if (vertex.get()->getOwner() == builder.get()->getNum()) {
        char c1 = builder.get()->getColour()[0];
        char c2 = vertex.get()->getImprovement();
        setChar(vertex.get()->getCol(), vertex.get()->getRow(), c1);
        setChar(vertex.get()->getCol() + 1, vertex.get()->getRow(), c2);
    }
}

void TextDisplay::buildRoad(shared_ptr<Edge> &edge, shared_ptr<Builder> &builder) {
    char colour = builder.get()->getColour()[0];
    setChar(edge.get()->getRow(), edge.get()->getCol(), colour);
    setChar(edge.get()->getRow() + 1, edge.get()->getCol(), 'R');
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
