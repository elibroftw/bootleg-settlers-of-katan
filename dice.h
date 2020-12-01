#ifndef __DICE__
#define __DICE__

class Dice {
    public:
        // rolls the fair dice, produces two random numbers
        // between 1 and 6, and returns the sum of the two
        unsigned int roll();
        // rolls the loaded dice, takes in input from the user
        // number entered must be in between 2 and 12
        unsigned int rollLoaded(); 
};

#endif
