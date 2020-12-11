#include <iostream>

#include "game.h"
#include "textdisplay.h"

using std::cout;

int main(int argc, char const *argv[]) {
    TextDisplay td;
    cout << td << std::endl;

    Game g;
    g.test();

    cout << "ALL TESTS COMPLETE" << std::endl;
    return 0;
}
