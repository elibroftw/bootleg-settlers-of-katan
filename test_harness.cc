#include "textdisplay.h"
#include <iostream>

using std::cout;

int main(int argc, char const *argv[]) {
    TextDisplay td = TextDisplay();
    cout << td << std::endl;
    cout << "ALL TESTS COMPLETE" << std::endl;
    return 0;
}