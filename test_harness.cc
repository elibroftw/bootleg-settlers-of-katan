#include <iostream>
#include <chrono>
#include "game.h"
#include "textdisplay.h"

using std::cout;

int main(int argc, char const *argv[]) {
    Vertex v1;
    Vertex v2 = v1;
    Vertex v3(5);
    v3 = v1;

    Edge e1;
    Edge e2 = e1;
    Edge e3(5);
    e3 = e1;

    Game g(std::chrono::system_clock::now().time_since_epoch().count());
    g.test();
    cout << "ALL TESTS COMPLETE" << std::endl;
    return 0;
}
