#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include "game.h"

using std::cin;
using std::cout;
using std::istringstream;
using std::string;

int main(int argc, char const *argv[]) {
    /* command line parsing */

    const string flagSeed = "-seed";
    const string flagLoad = "-load";
    const string flagBoard = "-board";
    const string randomBoard = "-random-board";

    bool nextIsSeed = false;
    bool nextIsSaveFile = false;
    bool nextIsLayoutFile = false;
    bool useRandomBoard = false;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    string saveFile = "";
    string layoutFile = "";

    for (int i = 1; i < argc; i++) {
        if (nextIsSeed) {
            nextIsSeed = false;
            istringstream ss{argv[i]};
            ss >> seed;
            // print error if seed is negative???
        } else if (nextIsSaveFile) {
            nextIsSaveFile = false;
            saveFile = argv[i];
        } else if (argv[i] == nextIsLayoutFile) {
            nextIsLayoutFile = false;
            layoutFile = argv[i];
        } else if (argv[i] == flagSeed) {
            nextIsSeed = true;
        } else if (argv[i] == flagLoad) {
            nextIsSaveFile = true;
        } else if (argv[i] == flagBoard) {
            nextIsLayoutFile = true;
        } else if (argv[i] == randomBoard) {
            useRandomBoard = layoutFile.empty() ? false : true;
        }
    }

    Game game = Game();
    // TODO: functions should check if file exists / is valid
    if (!saveFile.empty()) {
      game.loadGame(saveFile);
    } else if (!layoutFile.empty()) {
      game.createBoard(layoutFile);
    } else {
      game.createBoard();
    }

    if (!game.hasGameStarted()) {
      game.beginGame();
    }

    while (true) {
        // should game print stuff out???
        if (game.isGameOver()) {
            // prompt user to play again
            cout << "Would you like to play again?" << std::endl;
            string isYes;
            cin >> isYes;
            if(isYes == "yes" || isYes == "y" || isYes == "YES") {
                game.resetGame();
                game.beginGame();
            } else {
              return 0;
            }
        } else if (!game.nextTurn()) {
          // if cin fails, save to backup.sv
          game.saveGame("backup.sv");
          return 0;
        }
    }
    return 0;
}
