#include "game.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <unordered_set>

#include "builder.h"

// VM = verticiesMap
#define VM_WIDTH 6
#define VM_HEIGHT 11
#define NUM_BUILDERS 4

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::invalid_argument;
using std::locale;
using std::ofstream;
using std::pair;
using std::string;
using std::tie;
using std::toupper;
using std::unordered_set;

Game::Game() : curBuilder{-1}, geeseLocation{-1}, gameStarted{false}, gameOver{false} {
    // create builders
    builders.push_back(std::make_shared<Builder>("Blue"));
    builders.push_back(std::make_shared<Builder>("Red"));
    builders.push_back(std::make_shared<Builder>("Orange"));
    builders.push_back(std::make_shared<Builder>("Yellow"));

    verticesMap.resize(11);
    for (size_t row = 0; row < verticesMap.size(); row++) {
        verticesMap[row].resize(6);
    }

    // create 53 vertices
    for (int i = 0; i < 53; i++) {
        int xCoord;
        if (i == 2 || i == 48) {
            xCoord = 11;
        } else if (i == 0 || i == 3 || i == 49 || i == 52) {
            xCoord = 21;
        } else if (i == 1 || i == 4 || i == 50 || i == 53) {
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

        int vertexX, vertexY;
        tie(vertexX, vertexY) = getVertexFromCoords(xCoord, yCoord);
        verticesMap[vertexX][vertexY] = vertex;
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
        if (i == 0) {
            yCoord = 0;
        } else if (i == 1 || i == 2) {
            yCoord = 2;
        } else if (i == 3 || i == 4) {
            yCoord = 4;
        } else if (i >= 5 && i <= 8) {
            yCoord = 6;
        } else if (i >= 6 && i <= 11) {
            yCoord = 8;
        } else if (i >= 12 && i <= 17) {
            yCoord = 10;
        } else if (i >= 18 && i <= 19) {
            yCoord = 12;
        } else if (i >= 20 && i <= 25) {
            yCoord = 14;
        } else if (i >= 26 && i <= 28) {
            yCoord = 16;
        } else if (i >= 29 && i <= 34) {
            yCoord = 18;
        } else if (i >= 25 && i <= 36) {
            yCoord = 20;
        } else if (i >= 37 && i <= 42) {
            yCoord = 22;
        } else if (i >= 43 && i <= 45) {
            yCoord = 24;
        } else if (i >= 47 && i <= 51) {
            yCoord = 26;
        } else if (i >= 52 && i <= 53) {
            yCoord = 28;
        } else if (i >= 54 && i <= 59) {
            yCoord = 30;
        } else if (i >= 60 && i <= 62) {
            yCoord = 32;
        } else if (i >= 63 && i <= 66) {
            yCoord = 34;
        } else if (i >= 67 && i <= 68) {
            yCoord = 36;
        } else if (i == 69 || i == 70) {
            yCoord = 38;
        } else if (i == 71) {
            yCoord = 40;
        }
        auto edge = std::make_shared<Edge>(i, xCoord, yCoord, isHorizontal);
        edges.push_back(edge);
        edgesMap[xCoord / 5][yCoord / 2] = edge;
    }

    for (size_t i = 0; i < 18; i++) {
        auto tile = std::make_shared<Tile>(i);
        tiles.push_back(tile);
    }
}

pair<int, int> getVertexFromCoords(int x, int y) {
    return std::make_pair(x / 10, y / 4);
}

bool Game::hasGameStarted() {
    return gameStarted;
}

bool Game::isGameOver() {
    return gameOver;
}

string getResourceName(int resourceCode) {
    switch (resourceCode) {
        case 0:
            return "BRICKS";
        case 1:
            return "ENERGY";
        case 2:
            return "GLASS";
        case 3:
            return "HEAT";
        case 4:
            return "WIFI";
        default:
            return "PARK";
    }
}

void Game::createBoard() {
    // TODO: random board
}

void Game::createBoard(string filename) {
    ifstream file{filename};
    if (!file) {
        // if file exists or is invalid, create a random board
        createBoard();
        return;
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
            // more
        }
    } else {
        throw InvalidArgument();
    }
}

void Game::saveGame(string filename) {
    ofstream outfile{filename};
    outfile << curBuilder << endl;
    for (size_t i = 0; i < 4; i++) {
        outfile << builders[i].get() << endl;
    }
    for (size_t i = 0; i < tiles.size(); i++) {
        auto tile = tiles[i].get();
        if (i != 0) {
            outfile << " ";
        }
        outfile << tile->getResource() << " " << tile->getValue();
    }
    outfile << endl;
    if (geeseLocation != -1) {
        outfile << geeseLocation << endl;
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
        auto builder = builders[i];
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
                    builder.get()->setResource(i, std::stoi(temp));
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
        int vertexIdx;
        while (ss1 >> vertexIdx) {
            if (ss1 >> temp) {
                // call addHousing to Vertex
                auto vertex = vertices[vertexIdx];
                if (temp == "T") {
                    vertex.get()->upgradeResidence(builder, false);
                    temp = "H";
                }
                if (temp == "H") {
                    vertex.get()->upgradeResidence(builder, false);
                    temp = "B";
                }
                if (temp == "B") {
                    vertex.get()->upgradeResidence(builder, false);
                }
                buildLocations.push_back(vertexIdx);
            } else {
                throw InvalidSaveFile();
            }
        }
        // builder.get()->setBuilderPoints(bp);
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
    gameStarted = false;
}

bool Game::isValidVertex(shared_ptr<Vertex> vertex, bool considerEdges) {
    int xCoord = vertex.get()->getX() / 10;
    int yCoord = vertex.get()->getY() / 4;
    // check if adjacent verticies in the map have no owners

    for (size_t i = xCoord - 1; i < xCoord + 1; i++) {
        for (size_t j = yCoord - 1; j < yCoord + 1; j++) {
            if (i >= 0 && j >= 0 && i <= VM_HEIGHT && j <= VM_WIDTH && i != xCoord && j != yCoord) {
                if (verticesMap[i][j].get()->getOwner() != -1) {
                    // vertex is invalid since an adjacent vertex has an owner
                    return false;
                }
            }
        }
    }

    if (considerEdges) {
    }

    return true;
}

void Game::beginGame() {
    int vertexIdx;
    bool validVertex = false;
    for (int i = 0; i < 2; i++) {
        for (size_t j = 0; j < 4; j++) {
            while (!validVertex) {
                // accounts for reverse order
                auto builder = i == 0 ? builders[j] : builders[4 - j];
                cout << "Builder " << builder.get()->getColour()
                     << ", where do you want to build a basement?" << endl
                     << "> ";
                if (cin >> vertexIdx) {
                    if (vertexIdx >= 0 && vertexIdx <= 53) {
                        auto vertex = vertices[vertexIdx];
                        // build the basement
                        validVertex = isValidVertex(vertex, false);
                        if (validVertex) {
                            vertex.get()->upgradeResidence(builder, false);
                            buildLocations.push_back(vertexIdx);
                        } else {
                            cout << "You cannot build here." << endl;
                            cout << "Basements already exist as locations: ";
                            for (size_t x = 0; x < buildLocations.size(); x++) {
                                cout << buildLocations[x] << " ";
                            }
                            cout << endl;
                        }
                    }
                } else {
                    saveGame("backup.sv");
                    return;
                }
            }
        }
    }
    //
    gameStarted = true;
    // set curBuilder to Blue after "beginning of game"
    curBuilder = 0;
}

void Game::printBoard() { cout << textDisplay << endl; }


unordered_map<int, int> Game::getBuildersFromTile(int tileNumber) {
    auto coords = textDisplay.getTopLeftCoord(tileNumber);
    int xCoord = coords.first + 1;
    int yCoord = coords.second;
    unordered_map<int, int> buildersOnTile;
    for (size_t x = 0; x < 2; x++) {
        for (size_t y = 0; y < 3; j++) {
            auto vertex = verticesMap[xCoord + x][yCoord + y];
            int vertexOwner = vertex.get()->getOwner();
            if (vertexOwner >= 0) {
                int bp = vertex.get()->getBuildingPoints();

                if (buildersOnTile.count(vertexOwner)) {
                    buildersOnTile[vertexOwner] = bp;
                } else {
                    buildersOnTile[vertexOwner] += bp;
                }
            }
        }
    }
    return buildersOnTile;
}

bool Game::nextTurn() {
    // whenever a builder builds, check if builder has 10+ points
    // if it does set gameEnded to true
    cout << "Builder " << builders[curBuilder].get()->getColour() << "'s turn." << endl;

    // rolling the dice
    cout << "> ";
    bool rollDice = false;
    auto builder = builders[curBuilder].get();
    while (!rollDice) {
        string temp;
        if (!(cin >> temp)) {
            return false;
        }
        if (temp == "load" || temp == "l") {
            // set dice to laoded
            builder->useLoadedDice();
            cout << "Builder " << builder->getColour() << " now has a loaded Dice." << endl;
        } else if (temp == "fair" || temp == "f") {
            builder->useFairDice();
            cout << "Builder " << builder->getColour() << " now has a fair Dice." << endl;
            // set dice to fair
        } else if (temp == "roll" || temp == "r") {
            rollDice = true;
        } else if (temp == "status" || temp == "s") {
            for (size_t i = 0; i < 4; i++) {
                auto b = builders[i].get();
                string colour = b->getColour();
                int padding = 6 - colour.size();
                cout << "Builder " << colour << " ";
                for (size_t x = 0; x < padding; x++) {
                    cout << " ";
                }
                cout << "has " << b->getBuilderPoints() << " building points";
                for (size_t r = 0; r < 6; r++) {
                    cout << ", " << b->getResource(r) << getResourceName(r);
                }
            }
        } else if (temp == "help" || temp == "h") {
            cout << "load : changes current builder's dice type to 'loaded'" << endl;
            cout << "~ fair : changes current builder's dice type to 'fair'" << endl;
            cout << "~ roll : rolls the dice and distributes resources." << endl;
            cout << "~ status : prints the current status of all builders in order from builder 0 to 3." << endl;
            cout << "~ help : prints out the list of commands." << endl;
        } else {
            cout << "invalid Command." << endl;
            cout << "Please enter 'help' for a list of valid commands.";
        }
    }
    unsigned int diceVal = dice.roll(builder->isDiceLoaded());
    // if input failed
    if (diceVal <= 2) {
        return false;
    }

    // move geese
    if (diceVal == 7) {
        for (size_t i = 0; i < NUM_BUILDERS; i++) {
            builders[i].get()->geeseAttack();
        }
        cout << "Choose where to place the GEESE." << endl;
        unsigned int geeseLocation = 20;
        while (geeseLocation > 19) {
            if (!(cin >> geeseLocation)) {
                return false;
            }
            auto tile = tiles[geeseLocation];
            unordered_map<int, int> buildersOnTile = getBuildersFromTile(tile.get()->getNumber());
            bool printMsg = false;
            buildersOnTile.erase(curBuilder);
            for (auto const & [b, bp]  : buildersOnTile) {
                printMsg = true;
                cout << "Builder " << builder->getColour() << " can choose to steal from "
                     << builders[b].get()->getColour() << endl;
            }
            if (printMsg) {
                cout << "Choose a builder to steal from." << endl;
                locale loc;
                bool askForInput = true;
                while (askForInput) {
                    string input;
                    if (!(cin >> input)) {
                        return false;
                    }

                    toupper(input[0], loc);  // capitalize first letter
                    for (auto const & [b, bp] : buildersOnTile) {
                        auto tempBuilder = builders[b].get();
                        if (input[0] == tempBuilder->getColour()[0]) {
                            // TODO: check if correct
                            int stolenResource = tempBuilder->tryStealing();
                            if (stolenResource >= 0) {
                                cout << "Builder " << builder->getColour() << " stole "
                                     << getResourceName(stolenResource) << " from " << tempBuilder->getColour() << endl;
                                builder->setResource(stolenResource, builder->getResource(stolenResource) + 1);
                            }
                            askForInput = false;
                            break;
                        }
                    }
                }
            } else {
                cout << "No builders to steal from" << endl;
            }
        }

    } else {
        // use num to give builders resources
        vector<shared_ptr<Tile>> tilesWithValue;
        for (size_t i = 0; i < tiles.size(); i++) {
            auto tile = tiles[i];
            if (tile.get()->getValue() == diceVal) {
                unordered_map<int, int> buildersOnTile = getBuildersFromTile(tile.get()->getNumber());
                for (auto const & [b, bp] : buildersOnTile) {
                    // TODO
                }
                // distribute resources for all vertices and shit... you know the drill
                // get building points per builder in a map for the tile
            }
        }
    }

    bool endTurn = false;
    while (!endTurn) {
        string temp;
        if (!(cin >> temp)) {
            return false;
        } else if (temp == "help" || temp == "h") {
        }
    }

    ++curBuilder;
    return true;
}

void Game::tradeWith(Builder &builder, Resource resource1, Resource resource2) {
}

void Game::stealFrom(Builder &builder, Resource resource) {}

void Game::marketTrade(Resource resource1, Resource resource2) {}

void Game::resetGame() {
    curBuilder = -1;
    if (geeseLocation != -1) {
        tiles[geeseLocation].get()->removeGeese();
        textDisplay.removeGeese(geeseLocation);
    }

    for (size_t i = 0; i < 4; i++) {
        builders[i].get()->reset();
    }
    for (size_t i = 0; i < vertices.size(); i++) {
        auto vertex = vertices[i].get();
        vertex->reset();
        textDisplay.setInt(vertex->getX(), vertex->getY(), vertex->getNum());
    }

    for (size_t i = 0; i < edges.size(); i++) {
        auto edge = edges[i].get();
        edge->reset();
        textDisplay.setInt(edge->getX(), edge->getY(), edge->getNum());
    }

    geeseLocation = -1;
}
