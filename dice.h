#ifndef __DICE__
#define __DICE__
#include <random>

using std::default_random_engine;
using std::uniform_int_distribution;

class Dice {
    default_random_engine rng;
    uniform_int_distribution<unsigned> diceDistribution{2, 12};
    public:
        // sets engine with seed based on current time
        Dice();
        // rolls the fair dice, produces two random numbers
        // between 1 and 6, and returns the sum of the two
        // if loaded is true, rolls a loaded dice
        unsigned roll(bool loaded=false);
};

#endif
