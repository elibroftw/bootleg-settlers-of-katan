#ifndef __DICE_H__
#define __DICE_H__
#include <random>

using std::default_random_engine;
using std::uniform_int_distribution;

class Dice {
    uniform_int_distribution<unsigned> diceDistribution{1, 6};

   public:
    // rolls the fair dice, produces two random numbers
    // between 1 and 6, and returns the sum of the two
    // if loaded is true, rolls a loaded dice
    unsigned roll(default_random_engine &rng, const bool loaded = false);
};

#endif
