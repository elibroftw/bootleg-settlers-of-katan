#include "dice.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

unsigned int Dice::roll(bool loaded) {
    if (!loaded) {
        srand((int)time(0));
        int die1 = (rand() % 6) + 1;
        int die2 = (rand() % 6) + 1;
        return die1 + die2;
    }
    int loadedDie;
    while (true) {
        std::cout << "Input a roll between 2 and 12: ";

        if (!(std::cin >> loadedDie)) {
            // input failed therefore return an invalid roll
            return 0;
        }

        if (loadedDie >= 2 && loadedDie <= 12) {
            return loadedDie;
        }

        std::cout << "Invalid roll." << std::endl;
    }
}
