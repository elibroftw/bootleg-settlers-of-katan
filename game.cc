#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>

#include "builder.h"
#include "game.h"


using std::cout;
using std::cin;
using std::invalid_argument;
using std::ifstream;
using std::ofstream;
using std::string;

Game::Game() : curBuilder{0}, geeseLocation{-1} {
  // make a constructor with optional layout and optional import
  for (size_t i = 0; i < 4; i++) {
    string colour;
    switch (i) {
      case 0:
        colour = "Blue";
        break;
      case 1:
        colour = "Red";
        break;
      case 2:
        colour = "Orange";
        break;
      default:
        colour = "Yellow";
        break;
    }
    auto builder = std::make_shared<Builder>(colour);
    builders.push_back(builder);
  }

  verticesMap.resize(11);
  for (size_t row = 0; row < verticesMap.size(); row++) {
    // might need a Vertex default ctor (-1, -1, -1)
    verticesMap[row].resize(6);
  }

  // create 53 vertices
  for (int i = 0; i < 53; i++) {
    int xCoord;
    if (i == 2 || i == 48) {
      xCoord = 11;
    } else if (i == 0 || i == 3 || i == 49 || i == 52) {
      xCoord = 21;
    } else if (i == 1|| i == 4 || i == 50 || i == 53) {
      xCoord = 31;
    } else if (i == 5 || i == 51) {
      xCoord = 41;
    } else {  // i >=6 && i <= 47
      xCoord = (i % 6) * 10 + 1;
    }

    int yCoord;
    // set yCoord based on vertex number
    if (i == 0 || i == 1) {
      yCoord = 0;
    } else if (i == 52 || i == 53) {
      yCoord = 40;
    } else {
      yCoord = i / 6 * 4 + 4;
    }
    auto vertex = std::make_shared<Vertex>(i, xCoord, yCoord);
    // if vertex is a new
    verticesMap[xCoord / 10][yCoord / 4] = vertex;
  }

  edgesMap.resize(21);
  for (size_t row = 0; row < edgesMap.size(); row++) {
    // might need a Vertex default ctor (-1, -1, -1)
    edgesMap[row].resize(10);
  }

  // create 71 edges
  // edges can be vertical or horizontal
  for (int i = 0; i < 71; i++) {
    int xCoord;
    bool isHorizontal = false;
    if (i == 9 || i == 26 || i == 43 || i == 60) {
        xCoord = 6;
        isHorizontal = true;
    } else if (i == 3 || i == 18 || i == 35 || i == 52 || i == 67) {
        xCoord = 16;
        isHorizontal = true;
    } else if (i == 0 || i == 10 || i == 27 || i == 44 || i == 61 || i == 72) {
        xCoord = 26;
        isHorizontal = true;
    } else if (i == 4 || i == 19 || i == 36 || i == 53 || i == 69 || i == 68) {
        xCoord = 36;
        isHorizontal = true;
    } else if (i == 11 || i == 28 || i == 45 || i == 62) {
        xCoord = 42;
        isHorizontal = true;
    } else if (i == 12 || i == 20 || i == 29 || i == 37 || i == 46 || i == 54) {
        xCoord = 1;
    } else if (i == 5 || i == 13 || i == 21 || i == 30 || i == 38 || i == 47 ||
               i == 55 || i == 63) {
        xCoord = 11;
    } else if (i == 1 || i == 6 || i == 14 || i == 22 || i == 31 || i == 39 ||
                                                  i == 48 || i == 56 ||
                                                  i == 64 || i == 69) {
        xCoord = 21;
    } else if (i == 2 || i == 7 || i == 15 || i == 23 || i == 32 || i == 40 ||
                                                  i == 49 || i == 57 ||
                                                  i == 65 || i == 70) {
        xCoord = 31;
    } else if (i == 8 || i == 16 || i == 24 || i == 24 || i == 33 || i == 41 || i == 50 || i == 58 || i == 66) {
        xCoord = 41;
    } else if (i == 17 || i == 25 || i == 34 || i == 42 || i == 51 || i == 59) {
        xCoord = 51;
    }

    int yCoord;
    // set yCoord based on vertex number
    if (i == 0) { yCoord = 0; }
    else if (i == 1 || i == 2) { yCoord = 2; }
    else if (i == 3 || i == 4) { yCoord = 4; }
    else if (i >= 5 && i <= 8) { yCoord = 6; }
    else if (i >= 6 && i <= 11) { yCoord = 8; }
    else if (i >= 12 && i <= 17) { yCoord = 10; }
    else if (i >= 18 && i <= 19) { yCoord = 12; }
    else if (i >= 20 && i <= 25) { yCoord = 14; }
    else if (i >= 26 && i <= 28) { yCoord = 16; }
    else if (i >= 29 && i <= 34) { yCoord = 18; }
    else if (i >= 25 && i <= 36) { yCoord = 20; }
    else if (i >= 37 && i <= 42) { yCoord = 22; }
    else if (i >= 43 && i <= 45) { yCoord = 24; }
    else if (i >= 47 && i <= 51) { yCoord = 26; }
    else if (i >= 52 && i <= 53) { yCoord = 28; }
    else if (i >= 54 && i <= 59) { yCoord = 30; }
    else if (i >= 60 && i <= 62) { yCoord = 32; }
    else if (i >= 63 && i <= 66) { yCoord = 34; }
    else if (i >= 67 && i <= 68) { yCoord = 36; }
    else if (i == 69 || i == 70) { yCoord = 38; }
    else if (i == 71) { yCoord = 40; }
    auto edge = std::make_shared<Edge>(i, xCoord, yCoord, isHorizontal);
    edges.push_back(edge);
    edgesMap[xCoord / 5][yCoord / 2] = edge;
  }

  for (size_t i = 0; i < 18; i++)
  {
    auto tile = std::make_shared<Tile>(i);
    tiles.push_back(tile);
  }


  // vector<shared_ptr<Tile>> tiles;
}

void Game::createBoard() {
  // check if layout.txt exists
  try {
    createBoard("layout.txt");
  } catch (InvalidLayoutFile &e) {
    // random board????
  }
}

void Game::createBoard(string filename) {
  ifstream file{filename};
  if (!file) {
    throw InvalidLayoutFile();
  }
  int resource;
  int value;
  while (file >> resource) {
    if (file >> value) {
      auto tile = std::make_shared<Tile>(tiles.size(), value, resource);
      tiles.push_back(tile);
    }
  }
}

void Game::moveGeese(int to) {
  if (geeseLocation != to) {
    if (geeseLocation != -1) {
      textDisplay.removeGeese(geeseLocation);
      textDisplay.setGeese(to);
      geeseLocation = to;
    }
  } else {
    throw InvalidArgument();
  }
}

void Game::saveGame(string filename) {
  ofstream outfile{filename};
  outfile << curBuilder << std::endl;
  for (size_t i = 0; i < 4; i++) {
    outfile << builders[i].get() << std::endl;
  }
  for (size_t i = 0; i < tiles.size(); i++) {
    auto tile = tiles[i].get();
    if (i != 0) {
      outfile << " ";
    }
    outfile << tile->getResource() << " " << tile->getValue();
  }
  outfile << std::endl;
  if (geeseLocation != -1) {
    outfile << geeseLocation << std::endl;
  }
}

void Game::loadGame(string filename) {
  ifstream file{filename};
  if (!file) {
    throw InvalidSaveFile();
  }
  file >> curBuilder;
  string line;
  // read builder data for each buildere
  for (size_t i = 0; i < 4; i++) {
    // for each builder
    if (!getline(file, line)) {
      throw InvalidSaveFile();
    }
    auto builder = builders[i].get();
    std::istringstream ss1{line};
    string temp;
    bool readRoads = false;
    bool readHousing = false;
    // read resources
    for (size_t i = 0; i < 5; i++) {
      // read from sstream
      ss1 >> temp;
      if (temp == "r") {
        readRoads = true;
        break;
      } else {
        // if valid valid, try converting value to int
        try {
          builders[i].get()->setResource(i, std::stoi(temp));
        } catch (invalid_argument &e) {
          throw InvalidSaveFile();
        }
      }
    }

    // read roads
    if (readRoads) {
      while (true) {
        ss1 >> temp;
        if (temp == "h") {
          break;
        }
      }
    }

    // read housing
    int vertex;
    int bp = 0;  // keep track of added builder points
    while (ss1 >> vertex) {
      if (ss1 >> temp) {
        // call addHousing to Vertex
        if (temp == "B") {
            ++bp;
        } else if (temp == "H") {
            bp += 2;
        } else if (temp == "T") {
            bp += 3;
        }
      } else {
        throw InvalidSaveFile();
      }
    }
    builder->setBuilderPoints(bp);
  }

  // read board
  int resource;
  int value;
  if (!getline(file, line)) {
    throw InvalidSaveFile();
  }
  std::istringstream ss2{line};
  while (ss2 >> resource) {
    if (ss2 >> value) {
      auto tile = std::make_shared<Tile>(tiles.size(), value, resource);
      tiles.push_back(tile);
    }
  }

  // read geese location if present
  file >> geeseLocation;
}

bool Game::validVertex(Vertex &vertex) {
  // returns whether a basement can be built in here
}

void Game::newGame() {
  int vertex;
  bool validVertex = false;
  for (int i = 0; i < 2; i++) {
      for (size_t j = 0; j < 4; j++) {
          // account for reverse order
          while (!validVertex) {
              auto builder = i == 0 ? builders[j] : builders[4 - j];
              cout << "Builder " << builder.get()->getColour()
                   << ", where do you want to build a basement?" << std::endl
                   << "> ";
              cin >> vertex;
              if (vertex >= 0) {
                  // thoughts on vertex map
                  // have a vectoro of vectors filled with nullptrs/invalid vertex's
                  // e.g. 6x6
                  // some columns can't go left/right
                  // or manually create an algorithm to get adjacent verticices.

                  // 0 1
                  // 2 3 4 5
                  // 6 7 8 9 10 11
                  // 12 13 14 15 16 17
                  // 17 19 20 21 22 23
                  // 24 25 26 27 28 29
                  // 30 31 32 33 34 35
                  // 36 37 38 39 40 41
                  // 42 43 44 46 46 47
              }
          }
      }
  }

  // either run beginning mode now or after newGame() is called
}

void Game::printBoard() { cout << textDisplay << std::endl; }

void Game::nextTurn() {}

void Game::tradeWith(Builder &builder, Resource resource1, Resource resource2) {
}

void Game::endGame() {
  curBuilder = 0;
  if (geeseLocation != -1) {
    tiles[geeseLocation].get()->removeGeese();
  }

  for (size_t i = 0; i < 4; i++) {
    builders[i].get()->reset();
  }
  for (size_t i = 0; i < vertices.size(); i++) {
    vertices[i].get()->reset();
  }

  for (size_t i = 0; i < edges.size(); i++) {
    edges[i].get()->reset();
  }
  geeseLocation = -1;
  // reset textDisplay
}
void Game::stealFrom(Builder &builder, Resource resource) {}
bool Game::isGameOver() {}
void Game::marketTrade(Resource resource1, Resource resource2) {}
