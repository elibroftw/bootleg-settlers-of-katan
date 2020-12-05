#ifndef __DICE__
#define __DICE__

class Dice {
    public:
        // rolls the fair dice, produces two random numbers
        // between 1 and 6, and returns the sum of the two
        // if loaded is true, rolls a loaded dice
        unsigned int roll(bool loaded=false);
};

#endif
