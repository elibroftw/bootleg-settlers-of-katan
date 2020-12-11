#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include "game.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
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
    bool useLayoutFile = false;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    string saveFile = "";
    string layoutFile = "layout.txt";

    for (int i = 1; i < argc; i++) {
        if (nextIsSeed) {
            nextIsSeed = false;
            istringstream ss{argv[i]};
            ss >> seed;
        } else if (nextIsSaveFile) {
            nextIsSaveFile = false;
            saveFile = argv[i];
            cout << "f" << endl;
        } else if (nextIsLayoutFile) {
            nextIsLayoutFile = false;
            layoutFile = argv[i];
            useLayoutFile = true;
            useRandomBoard = false;
        } else if (argv[i] == flagSeed) {
            nextIsSeed = true;
        } else if (argv[i] == flagLoad) {
            nextIsSaveFile = true;
        } else if (argv[i] == flagBoard) {
            nextIsLayoutFile = true;
        } else if (argv[i] == randomBoard) {
            useRandomBoard = useLayoutFile == false;
        }
    }
    cout << "Settlers of Waterloo By Elijah Lopez and Abdullah Hadi" << endl;

    Game game;
    cout << "Loading game from " << saveFile << endl;
    if (!saveFile.empty()) {
        cout << "Loading game from " << saveFile << endl;
        game.loadGame(saveFile);
    } else if (useRandomBoard) {
        game.createBoard(seed);
    } else {
        game.createBoard(layoutFile);
    }

    if (!game.hasGameStarted()) {
        // quit if begin game failed
        if (!game.beginGame()) {
            cerr << "failed to begin game, please try again" << endl;
            return 0;
        }
    }

    while (true) {
        // should game print stuff out???
        if (game.isGameOver()) {
            // prompt user to play again
            cout << "Would you like to play again?" << std::endl;
            string isYes;
            cin >> isYes;
            if (isYes == "yes" || isYes == "y" || isYes == "YES") {
                game.resetGame();
                // quit if begin game failed
                if (!game.beginGame()) return 0;
            } else {
                return 0;
            }
        } else if (!game.nextTurn()) {
            // nextTurn was unsuccesful, so save and exit gracefully
            game.saveGame("backup.sv");
            return 0;
        }
    }
    return 0;
}
