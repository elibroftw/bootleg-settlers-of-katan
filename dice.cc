#include "dice.h"

#include <chrono>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

Dice::Dice() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng = default_random_engine{seed};
}

unsigned Dice::roll(bool loaded) {
    if (!loaded) {
        unsigned val = diceDistribution(rng) + diceDistribution(rng);
        cout << "You rolled a " << val << "." << endl;
        return val;
    }

    int loadedDie = 0;
    while (true) {
        cout << "Input a roll between 2 and 12:" << endl
             << "> ";

        // EOF detected therefore return an invalid roll
        if (!(cin >> loadedDie)) {
            if (cin.eof()) {
                return 0;
            }
            cin.clear();
            cin.ignore(1000, '\n');
        }

        if (loadedDie >= 2 && loadedDie <= 12) return loadedDie;

        cout << "Invalid roll." << endl;
    }
}
