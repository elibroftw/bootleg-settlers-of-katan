#include "textdisplay.h"

using std::ostream;

TextDisplay::TextDisplay() {
    // add default chars to board here?
}

void TextDisplay::setChar(int x, int y, char c) {
    // assumes x >= 0 and y >= 0;
    board[x][y] = c;
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
