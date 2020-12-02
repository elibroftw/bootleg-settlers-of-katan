#include "game.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "builder.h"

using std::cout;
using std::ifstream;
using std::ofstream;
using std::string;

Game::Game() builders{}, curBuilder{0}, tiles{}, vertices{}, edges{},
    textDisplay{}, geeseLocation{-1} {
  // make a constructor with optional layout and optional import
  for (unsigned_int i = 0; i < 4; i++) {
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

  // vector<shared_pointer<Tile>> tiles;
  // vector<shared_pointer<Vertex>> vertices;
  // vector<shared_pointer<Edge>> edges;
}

void Game::createBoard() {
  // check if layout.txt exists
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
      auto tile = make_shared<Tile>(tiles.size(), value, resource);
      tiles.push_back(tile);
    }
  }
}

void Game::moveGeese(int to) {}

void Game::saveGame(string filename) {
  ofstream outfile{filename};
  outfile << i << std::endl;
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
  file >> curTurn;
  string line;
  // read builder data for each buildere
  for (size_t i = 0; i < 4; i++) {
    // for each builder
    if (!getline(file, line)) {
      throw InvalidSaveFile();
    }
    auto builder = builders[i].get();
    std::ostringstream ss1{line};
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
        switch (temp) {
          case "B":
            ++bp;
            break;
          case "H":
            bp += 2;
            break;
          case "T":
            bp += 3;
            break;
          default:
            break;
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
  while (line >> resource) {
    if (line >> value) {
      auto tile = make_shared<Tile>(tiles.size(), value, resource);
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
      while (!validVertex)
      auto builder = i == 0 ? builders[j] : builders[4 - j];
      cout << "Builder " << builder.get()->getColour()
           << ", where do you want to build a basement?" << std::endl << "> ";
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
