#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "dice.h"

unsigned int Dice::roll() {
    srand((unsigned) time(0));
    unsigned int die1 = (rand() % 6) + 1;
    unsigned int die2 = (rand() % 6) + 1;
    unsigned int sum = die1 + die2;
    return sum;
}

unsigned int Dice::rollLoaded() {
    bool valid = false;
    int loadedDie;
    while(!valid) {
        std::cout << "Input a roll between 2 and 12: ";
        // TODO: raise an exception
        if (std::cin >> loadedDie) {

        }
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(256,'\n');
        }

        if (loadedDie >= 2 && loadedDie <= 12) {
            valid = true;
            continue;
        }

        std::cout << "Invalid roll." << std::endl;
    }
    return loadedDie;
}
