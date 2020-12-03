#include "game.h"
#include <iostream>

using std::cin;
using std::cout;

int main(int argc, char const *argv[]) {
  /* command line parsing */

  Game game = Game();
  // game.createBoard();
  // game.beginGame();
  while(true) {

    if (game.isGameOver()) {
      // if user says new game
      bool newGameSelected = false;
      if (newGameSelected) {
        game.resetGame();
        game.beginGame();
      }
    }
  }
  return 0;
}
