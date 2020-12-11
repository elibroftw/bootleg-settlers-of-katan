#include "dice.h"

#include <chrono>
#include <iostream>
#include <random>
#include <string>

using std::cin;
using std::cout;
using std::default_random_engine;
using std::endl;
using std::uniform_int_distribution;

Dice::Dice() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng = default_random_engine{seed};
}

unsigned Dice::roll(bool loaded) {
    if (!loaded) return diceDistribution(rng) + diceDistribution(rng);

    int loadedDie = 0;
    while (true) {
        cout << "Input a roll between 2 and 12: ";

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
