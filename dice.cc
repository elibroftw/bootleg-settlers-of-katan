#include "dice.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

unsigned Dice::roll(bool loaded) {
    if (!loaded) {
        srand((int)time(0));
        int die1 = (rand() % 6) + 1;
        int die2 = (rand() % 6) + 1;
        return die1 + die2;
    }
    int loadedDie = 0;
    while (true) {
        cout << "Input a roll between 2 and 12: ";

        // EOF detected therefore return an invalid roll
        if (!(cin >> loadedDie) && cin.eof()) return 0;

        if (loadedDie >= 2 && loadedDie <= 12) return loadedDie;

        cout << "Invalid roll." << endl;
    }
}
