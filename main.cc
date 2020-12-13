#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include "game.h"

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::istringstream;
using std::string;

int main(int argc, char const *argv[]) {
    /* command line parsing */

    const string flagSeedLong = "-seed";
    const string flagSeedShort = "-s";
    bool nextIsSeed = false;

    const string flagLoadLong = "-load";
    const string flagLoadShort = "-l";
    bool nextIsSaveFile = false;

    const string flagBoardLong = "-board";
    const string flagBoardShort = "-b";
    bool nextIsLayoutFile = false;
    bool useLayoutFile = false;

    const string flagRandomLong = "-random-board";
    const string flagRandomShort = "-r";
    bool useRandomBoard = false;

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
        } else if (nextIsLayoutFile) {
            layoutFile = argv[i];
            nextIsLayoutFile = false;
            useLayoutFile = true;
            useRandomBoard = false;
        } else if (argv[i] == flagSeedLong || argv[i] == flagSeedShort) {
            nextIsSeed = true;
        } else if (argv[i] == flagLoadLong || argv[i] == flagLoadShort) {
            nextIsSaveFile = true;
        } else if (argv[i] == flagBoardLong || argv[i] == flagBoardShort) {
            nextIsLayoutFile = true;
        } else if (argv[i] == flagRandomLong || argv[i] == flagRandomShort) {
            useRandomBoard = useLayoutFile == false;
        }
    }
    cout << "Settlers of Waterloo By Elijah Lopez and Abdullah Hadi" << endl;

    Game game;
    if (!saveFile.empty()) {
        try {
            game.load(saveFile);
        } catch (InvalidSaveFile &e) {
            game.reset();
            cout << "Starting a new game." << endl;
            game.createBoard(seed);
        }
    } else if (useRandomBoard) {
        game.createBoard(seed);
    } else {
        game.createBoard(layoutFile);
    }

    if (!game.hasGameStarted()) {
        // quit if begin game failed
        if (!game.beginGame()) {
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
                game.reset();
                // quit if begin game failed
                if (!game.beginGame()) return 0;
            } else {
                return 0;
            }
        } else if (!game.nextTurn()) {
            // nextTurn was unsuccesful, so save and exit gracefully
            game.save("backup.sv");
            return 0;
        }
    }
    return 0;
}
