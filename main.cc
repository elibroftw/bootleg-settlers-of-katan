#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include "game.h"

using std::cin;
using std::cout;
using std::endl;
using std::istringstream;

int main(int argc, char const *argv[]) {
    // print out game info
    const string gameName = "Settlers of Katan - Bootleg Edition";
    const string authors = "Elijah Lopez and Abdullah Hadi";
    cout << gameName << " By " << authors << endl;

    // command line parsing
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

    const string flagHelpLong = "-help";
    const string flagHelpShort = "-h";
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
        } else if (argv[i] == flagHelpLong || argv[i] == flagHelpShort) {
            cout << "-load <savefile>   : load game from a save file" << endl;
            cout << "-board <layout>    : start game using board from a layout file" << endl;
            cout << "-seed <seed>       : seed to be used when creating a random board" << endl;
            cout << "-random-board      : start game using a random board" << endl;
            return 0;
        }
    }

    Game game(seed);
    if (!saveFile.empty()) {
        try {
            game.load(saveFile);
        } catch (InvalidSaveFile &e) {
            game.reset();
            cout << "Starting a new game." << endl;
            game.createBoard();
        }
    } else if (useRandomBoard) {
        game.createBoard();
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
            cout << "Would you like to play again?" << endl
                 << "> ";
            string isYes;
            cin >> isYes;
            if (isYes == "yes" || isYes == "y" || isYes == "YES") {
                game.reset();
                // quit if begin game failed
                if (!game.beginGame()) return 0;
            } else {
                cout << "Thanks for playing " << gameName << endl;
                return 0;
            }
        } else if (!game.nextTurn()) {  // EOF detected
            game.save("backup.sv");
            return 0;
        }
    }
    return 0;
}
