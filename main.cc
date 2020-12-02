#include "game.h"

int main(int argc, char const *argv[]) {
  /* command line parsing */


  Game game = Game();
  try {
    while (true) {
        // read input from user
    }
  } catch (ios::failure &) {
      game.saveGame("backup.sv");
  }
  return 0;
}
