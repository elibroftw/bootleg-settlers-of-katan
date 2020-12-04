#include "vertex.h"

Vertex::Vertex(int number, int xCoord, int yCoord) {
    this->number =  number;
    this->xCoord = xCoord;
    this->yCoord = yCoord;
}

int Vertex::getX() {
    return xCoord;
}

int Vertex::getY() {
    return yCoord;
}

int Vertex::getNum() {
    return number;
}

int Vertex::getOwner() {
    return owner;
}

void Vertex::addBasement(shared_ptr<Builder> builder, bool checkResources) {

}
