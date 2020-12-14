#include "dice.h"

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

unsigned Dice::roll(default_random_engine &rng, const bool loaded) {
    if (!loaded) {
        unsigned val;
        for (size_t i = 0; i < 1000; i++) {
            val = diceDistribution(rng) + diceDistribution(rng);
        }
        cout << "You rolled the number: " << val << "." << endl;
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
