#ifndef __EDGE__
#define __EDGE__

class Edge {
    int owner;
    int number;
    // the x and y coordinates are for the text display
    int xCoord;
    int yCoord;
    bool isRoad;

   public:
    Edge(int number = -1, int xCoord = -1, int yCoord = -1);

    int getX();
    int getY();
    int getNum();
    // Used to build a road at an edge. Each edge is limited
    // to one road, and costs the builder one heat and wifi resource.
    // A road can be built by a builder if an adjacent road or
    // residence has been built by the same builder.
    void buildRoad(int owner);

    // resets owner to -1 and improvement to ""
    void reset();

    // returns whether the edge has valid number, xCoord, and yCoord
    bool realEdge();
};

#endif
