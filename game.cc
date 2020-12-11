#include "game.h"

#include <algorithm>
#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_set>

#include "builder.h"
#include "edge.h"
#include "vertex.h"

// VM = verticiesMap
#define VM_HEIGHT 11
#define VM_WIDTH 6
// EM = edgesMap
#define EM_HEIGHT 21
#define EM_WIDTH 10
// text display height and width
#define TD_HEIGHT 41
#define TD_WIDTH 54

#define NUM_BUILDERS 4

using std::cerr;
using std::cin;
using std::cout;
using std::default_random_engine;
using std::endl;
using std::ifstream;
using std::invalid_argument;
using std::make_pair;
using std::ofstream;
using std::pair;
using std::random_shuffle;
using std::string;
using std::tie;
using std::toupper;
using std::uniform_int_distribution;
using std::unordered_map;
using std::unordered_set;

Game::Game() : curTurn{-1}, geeseLocation{19}, gameStarted{false}, gameOver{false} {
    // create builders
    builders.push_back(std::make_shared<Builder>("Blue", 0));
    builders.push_back(std::make_shared<Builder>("Red", 1));
    builders.push_back(std::make_shared<Builder>("Orange", 2));
    builders.push_back(std::make_shared<Builder>("Yellow", 3));

    verticesMap.resize(VM_HEIGHT);
    for (size_t row = 0; row < verticesMap.size(); row++) {
        verticesMap[row].resize(VM_WIDTH);
    }

    // create 53 vertices
    for (int i = 0; i < 53; i++) {
        int colTD;
        if (i == 2 || i == 48) {
            colTD = 11;
        } else if (i == 0 || i == 3 || i == 49 || i == 52) {
            colTD = 21;
        } else if (i == 1 || i == 4 || i == 50 || i == 53) {
            colTD = 31;
        } else if (i == 5 || i == 51) {
            colTD = 41;
        } else {  // i >=6 && i <= 47
            colTD = (i % 6) * 10 + 1;
        }

        int rowTD;
        // set rowTD based on vertex number
        if (i == 0 || i == 1) {
            rowTD = 0;
        } else if (i == 52 || i == 53) {
            rowTD = 40;
        } else {
            rowTD = i / 6 * 4 + 4;
        }
        auto vertex = std::make_shared<Vertex>(i, rowTD, colTD);
        vertices.push_back(vertex);
        int vertexR, vertexC;
        tie(vertexR, vertexC) = Vertex::getVertexFromCoords(rowTD, colTD);
        verticesMap[vertexR][vertexC] = vertex;
    }

    edgesMap.resize(EM_HEIGHT);
    for (size_t row = 0; row < edgesMap.size(); row++) {
        edgesMap[row].resize(EM_WIDTH);
    }

    // create 71 edges
    // edges can be vertical or horizontal
    for (int i = 0; i < 71; i++) {
        int colTD = 0;
        bool isHorizontal = false;
        if (i == 9 || i == 26 || i == 43 || i == 60) {
            colTD = 6;
            isHorizontal = true;
        } else if (i == 3 || i == 18 || i == 35 || i == 52 || i == 67) {
            colTD = 16;
            isHorizontal = true;
        } else if (i == 0 || i == 10 || i == 27 || i == 44 || i == 61 || i == 72) {
            colTD = 26;
            isHorizontal = true;
        } else if (i == 4 || i == 19 || i == 36 || i == 53 || i == 69 || i == 68) {
            colTD = 36;
            isHorizontal = true;
        } else if (i == 11 || i == 28 || i == 45 || i == 62) {
            colTD = 42;
            isHorizontal = true;
        } else if (i == 12 || i == 20 || i == 29 || i == 37 || i == 46 || i == 54) {
            colTD = 1;
        } else if (i == 5 || i == 13 || i == 21 || i == 30 || i == 38 || i == 47 ||
                   i == 55 || i == 63) {
            colTD = 11;
        } else if (i == 1 || i == 6 || i == 14 || i == 22 || i == 31 || i == 39 ||
                   i == 48 || i == 56 ||
                   i == 64 || i == 69) {
            colTD = 21;
        } else if (i == 2 || i == 7 || i == 15 || i == 23 || i == 32 || i == 40 ||
                   i == 49 || i == 57 ||
                   i == 65 || i == 70) {
            colTD = 31;
        } else if (i == 8 || i == 16 || i == 24 || i == 24 || i == 33 || i == 41 || i == 50 || i == 58 || i == 66) {
            colTD = 41;
        } else if (i == 17 || i == 25 || i == 34 || i == 42 || i == 51 || i == 59) {
            colTD = 51;
        } else {
            cerr << "SOMETHING WENT WRONG" << endl;
        }

        int rowTD = 0;
        // set colTD based on edge number
        if (i == 0) {
            rowTD = 0;
        } else if (i == 1 || i == 2) {
            rowTD = 2;
        } else if (i == 3 || i == 4) {
            rowTD = 4;
        } else if (i >= 5 && i <= 8) {
            rowTD = 6;
        } else if (i >= 6 && i <= 11) {
            rowTD = 8;
        } else if (i >= 12 && i <= 17) {
            rowTD = 10;
        } else if (i >= 18 && i <= 19) {
            rowTD = 12;
        } else if (i >= 20 && i <= 25) {
            rowTD = 14;
        } else if (i >= 26 && i <= 28) {
            rowTD = 16;
        } else if (i >= 29 && i <= 34) {
            rowTD = 18;
        } else if (i >= 25 && i <= 36) {
            rowTD = 20;
        } else if (i >= 37 && i <= 42) {
            rowTD = 22;
        } else if (i >= 43 && i <= 45) {
            rowTD = 24;
        } else if (i >= 47 && i <= 51) {
            rowTD = 26;
        } else if (i >= 52 && i <= 53) {
            rowTD = 28;
        } else if (i >= 54 && i <= 59) {
            rowTD = 30;
        } else if (i >= 60 && i <= 62) {
            rowTD = 32;
        } else if (i >= 63 && i <= 66) {
            rowTD = 34;
        } else if (i >= 67 && i <= 68) {
            rowTD = 36;
        } else if (i == 69 || i == 70) {
            rowTD = 38;
        } else if (i == 71) {
            rowTD = 40;
        } else {
            cerr << "SOMETHING WENT WRONG SETTING ROW OF EDGE" << endl;
        }
        auto edge = std::make_shared<Edge>(i, colTD, rowTD, isHorizontal);
        edges.push_back(edge);
        int edgeR, edgeC;
        tie(edgeR, edgeC) = Edge::getEdgeFromCoords(rowTD, colTD);
        edgesMap[edgeR][edgeC] = edge;
    }
}

bool Game::hasGameStarted() {
    return gameStarted;
}

bool Game::isGameOver() {
    return gameOver;
}

void Game::createBoard(unsigned seed) {
    // use a time-based seed for the default seed value
    default_random_engine rng{seed};
    uniform_int_distribution<unsigned> distribAll{0, 18};
    uniform_int_distribution<unsigned> distrib1{3, 6};
    uniform_int_distribution<unsigned> distrib2{8, 11};

    vector<unsigned> randomResources = {Wifi, Wifi, Wifi, Heat, Heat, Heat,
                                        Brick, Brick, Brick, Brick, Energy,
                                        Energy, Energy, Energy, Glass, Glass, Glass};
    vector<unsigned> randomValues;
    for (size_t i = 0; i < 3; i++) {
        randomValues.push_back(2);
        randomValues.push_back(distrib1(rng));
        randomValues.push_back(distrib1(rng));
        randomValues.push_back(distrib2(rng));
        randomValues.push_back(distrib2(rng));
        randomValues.push_back(12);
    }
    // shuffle here
    random_shuffle(randomResources.begin(), randomResources.end());
    random_shuffle(randomValues.begin(), randomValues.end());

    // create random index from 0 to 18 to insert Park and value of 7
    unsigned parkIndex = distribAll(rng);
    randomResources.insert(randomResources.begin() + parkIndex, Park);
    randomValues.insert(randomValues.begin() + parkIndex, 7);

    for (size_t i = 0; i < 19; i++) {
        auto tile = std::make_shared<Tile>(i, randomValues[i], randomResources[i]);
        tiles.push_back(tile);
    }
}

void Game::createBoard(string filename) {
    ifstream file{filename};
    if (!file) {
        // if file does not exist or is invalid, create a random board
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        createBoard(seed);
        return;
    }
    unsigned resource;
    unsigned value;
    while (file >> resource) {
        if (file >> value) {
            auto tile = std::make_shared<Tile>(tiles.size(), value, resource);
            textDisplay.setTileResource(tiles.size(), getResourceName(resource));
            textDisplay.setTileValue(tiles.size(), value);
            tiles.push_back(tile);
        } else {
            cerr << "ERROR: layout.txt was not formatted correctly" << endl;
        }
    }
}

void Game::saveGame(string filename) {
    ofstream outfile{filename};
    outfile << curTurn << endl;

    unordered_map<int, vector<int>> roads;
    for (auto &&i : roadLocations) {
        int owner = edges[i].get()->getOwner();
        if (roads.count(owner)) {
            roads[owner].push_back(i);
        } else {
            roads.insert(make_pair(owner, vector<int>{i}));
        }
    }

    unordered_map<int, vector<int>> housing;
    for (auto &&i : resLocations) {
        int owner = vertices[i].get()->getOwner();
        if (housing.count(owner)) {
            housing[owner].push_back(i);
        } else {
            housing.insert(make_pair(owner, vector<int>{i}));
        }
    }

    for (size_t i = 0; i < 4; i++) {
        builders[i].get()->printResources(outfile);
        outfile << "r";
        for (auto &&r : roads[i]) {
            outfile << ' ' << r;
        }
        outfile << " h";
        for (auto &&v : housing[i]) {
            auto vertex = vertices[v].get();
            outfile << ' ' << vertex->getNum() << ' ' << vertex->getImprovement();
        }
        outfile << endl;
    }

    for (size_t i = 0; i < tiles.size(); i++) {
        auto tile = tiles[i].get();
        if (i != 0) {
            outfile << " ";
        }
        outfile << getResourceName(tile->getResource()) << " " << tile->getValue();
    }
    outfile << endl;
    if (geeseLocation < 19) {
        outfile << geeseLocation << endl;
    }
}

void Game::loadGame(string filename) {
    ifstream file{filename};
    if (!file) {
        cerr << "ERROR: Invalid Save File" << endl;
        throw InvalidSaveFile();
    }
    file >> curTurn;
    string line;
    // read builder data for each buildere
    for (size_t i = 0; i < 4; i++) {
        // for each builder
        if (!getline(file, line)) {
            cerr << "ERROR: Invalid Save File" << endl;
            throw InvalidSaveFile();
        }
        auto builder = builders[i];
        std::istringstream ss1{line};
        string temp;
        // read resources
        for (size_t i = 0; i < 5; i++) {
            // read from sstream
            ss1 >> temp;
            if (temp == "r") {
                break;
            } else {
                // if read was valid, try converting value to int
                try {
                    builder.get()->setResource(i, std::stoi(temp));
                } catch (invalid_argument &e) {
                    cerr << "ERROR: Invalid Save File" << endl;
                    throw InvalidSaveFile();
                }
            }
        }

        // read roads
        while (true) {
            ss1 >> temp;
            if (temp == "h") {
                break;
            } else {
                int roadIdx = std::stoi(temp);
                auto edge = edges[roadIdx];
                edge.get()->buildRoad(builder, false);
                textDisplay.buildRoad(edge, builder);
                roadLocations.push_back(roadIdx);
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
                textDisplay.updateVertex(vertex, builder);
                resLocations.push_back(vertexIdx);
            } else {
                cerr << "ERROR: Invalid Save File" << endl;
                throw InvalidSaveFile();
            }
        }
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
            textDisplay.setTileResource(tiles.size(), getResourceName(resource));
            textDisplay.setTileValue(tiles.size(), value);
            tiles.push_back(tile);
        }
    }

    // read geese location if present
    file >> geeseLocation;
    gameStarted = true;
}

bool Game::isValidVertex(shared_ptr<Vertex> vertex, bool considerEdges) {
    int col = vertex.get()->getCol() / 10;
    int row = vertex.get()->getRow() / 4;
    // check if it and adjacent verticies in the map have no owners
    for (int r = row - 1; r < row + 1; r++) {
        // check if vertex above, below and itself have owners
        if (r >= 0 && r <= VM_HEIGHT && verticesMap[r][col].get()->getOwner() != -1) {
            // vertex is invalid since it or an vertex above/below has an owner
            return false;
        }
    }
    int c;
    int vertexNum = vertex.get()->getNum();
    bool leftIsFlat = false;
    // get location of left/right vertex
    if ((vertexNum >= 13 && vertexNum <= 16) ||
        (vertexNum >= 25 && vertexNum <= 28) || (vertexNum >= 37 && vertexNum <= 40)) {
        c = col + (vertexNum % 2 ? 1 : -1);
        leftIsFlat = vertexNum % 2 ? true : false;
    } else {
        c = col + (vertexNum % 2 ? -1 : 1);
        leftIsFlat = vertexNum % 2 ? false : true;
    }

    // check if vertex directly left/right has a residence
    if (c >= 0 && c <= VM_WIDTH) {
        if (verticesMap[row][c].get()->getOwner() != -1) {
            return false;
        }
    }

    // look for adjacent roads
    if (considerEdges) {
        int edgeR, edgeC;

        if (row + 2 < TD_HEIGHT) {
            // check if edge above is a road
            tie(edgeR, edgeC) = Edge::getEdgeFromCoords(row + 2, col);
            if (edgesMap[edgeR][edgeC].get()->getOwner() == curTurn) return true;
        }

        if (row - 2 >= 0) {
            // check if edge below is a road
            tie(edgeR, edgeC) = Edge::getEdgeFromCoords(row - 2, col);
            if (edgesMap[edgeR][edgeC].get()->getOwner() == curTurn) return true;
        }

        if (leftIsFlat) {
            // since left is flat, check if edge to the right is flat
            tie(edgeR, edgeC) = Edge::getEdgeFromCoords(row, col + 5);
            if (edgesMap[edgeR][edgeC].get()->getOwner() == curTurn) return true;
        } else {
            // since right is falt, check if edge to the lefet is flat
            tie(edgeR, edgeC) = Edge::getEdgeFromCoords(row, col - 5);
            if (edgesMap[edgeR][edgeC].get()->getOwner() == curTurn) return true;
        }
        return false;
    }
    return true;
}

bool Game::isValidEdge(shared_ptr<Edge> edge) {
    int rowTD = edge.get()->getRow();
    int row = rowTD / 10;
    int colTD = edge.get()->getCol();
    int col = colTD / 4;
    // can't build on a road
    if (edge.get()->getOwner() != -1) return false;

    int vertexR, vertexC;
    if (edge.get()->getHorizontal()) {
        // check the vertecies left and right of 5 units
        for (int colDelta = -5; colDelta <= 5; colDelta += 10) {
            tie(vertexR, vertexC) = Vertex::getVertexFromCoords(rowTD, colTD + colDelta);
            auto vertex = verticesMap[vertexR][vertexC].get();
            if (vertex->getOwner() == curTurn) return true;
            else if (vertex->getOwner() == -1) {
                // check if edges up and below are valid, given vertex is not blocked
                int c = col + colDelta / 5;
                for (int r = row - 1; r <= row + 1; r += 2) {
                    if (r >= 0 && r <= EM_HEIGHT) {
                        if (edgesMap[r][c].get()->getOwner() == curTurn) {
                            // edge is valid since an adjacent edge is a road
                            return true;
                        }
                    }
                }
            }
        }
    } else {
        // check the vertecies above and below of 2 units
        for (int rowDelta = -2; rowDelta <= 2; rowDelta += 4) {
            tie(vertexR, vertexC) = Vertex::getVertexFromCoords(rowTD + rowDelta, colTD);
            auto vertex = verticesMap[vertexR][vertexC].get();
            if (vertex->getOwner() == curTurn) return true;
            else if (vertex->getOwner() == -1) {
                // check if edges above, and right are valid, given vertex is not blocked
                int r = row + rowDelta;
                if (r >= 0 && r <= EM_HEIGHT) {
                    if (edgesMap[r][col].get()->getOwner() == curTurn) {
                        return true;
                    }
                }
                r -= rowDelta / 2;
                if (r >= 0 && r <= EM_HEIGHT) {
                    for (int c = row - 1; c <= row + 1; c += 2) {
                        if (c >= 0 && c <= EM_WIDTH) {
                            if (edgesMap[r][c].get()->getOwner() == curTurn) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Game::beginGame() {
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
                            resLocations.push_back(vertexIdx);
                        } else {
                            cout << "You cannot build here." << endl;
                            cout << "Basements already exist as locations: ";
                            for (size_t k = 0; k < resLocations.size(); k++) {
                                cout << resLocations[k] << " ";
                            }
                            cout << endl;
                        }
                    }
                } else if (cin.eof()) {
                    saveGame("backup.sv");
                    return false;
                }
            }
        }
    }
    //
    gameStarted = true;
    // set curBuilder to Blue after "beginning of game"
    curTurn = 0;
    return true;
}

void Game::printBoard() { cout << textDisplay << endl; }

unordered_map<int, int> Game::getBuildersFromTile(int tileNumber) {
    int row, col;
    tie(row, col) = textDisplay.getTopLeftCoord(tileNumber);
    ++row;
    unordered_map<int, int> buildersOnTile;
    for (size_t r = 0; r < 2; r++) {
        for (size_t c = 0; c < 3; c++) {
            auto vertex = verticesMap[row + r][col + c];
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

void Game::printStatus() {
    for (size_t i = 0; i < 4; i++) {
        auto b = builders[i].get();
        string colour = b->getColour();
        unsigned padding = 6 - colour.size();
        cout << "Builder " << colour << " ";
        for (size_t j = 0; j < padding; j++) {
            cout << " ";
        }
        cout << "has " << b->getBuildingPoints() << " building points";
        for (size_t r = 0; r < 6; r++) {
            cout << ", " << b->getResource(r) << getResourceName(r);
        }
    }
}

bool Game::nextTurn() {
    // whenever a builder builds, check if builder has 10+ points
    // if it does set gameEnded to true
    cout << "Builder " << builders[curTurn].get()->getColour() << "'s turn." << endl;

    // rolling the dice
    cout << "> ";
    bool rollDice = false;
    auto builderShared = builders[curTurn];
    auto builder = builderShared.get();
    while (!rollDice) {
        string temp;
        if (!(cin >> temp) && cin.eof()) {
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
            printStatus();
        } else if (temp == "help" || temp == "h") {
            cout << "~ load : changes current builder's dice type to 'loaded'" << endl;
            cout << "~ fair : changes current builder's dice type to 'fair'" << endl;
            cout << "~ roll : rolls the dice and distributes resources." << endl;
            cout << "~ status : prints the current status of all builders in order from builder 0 to 3." << endl;
            cout << "~ help : prints out the list of commands." << endl;
        } else {
            cout << "Invalid Command." << endl;
            cout << "Please enter 'help' for a list of valid commands.";
        }
    }
    unsigned diceVal = dice.roll(builder->isDiceLoaded());
    // if EOF detected
    if (diceVal <= 2) {
        return false;
    }

    // geese
    if (diceVal == 7) {
        for (size_t i = 0; i < NUM_BUILDERS; i++) {
            builders[i].get()->geeseAttack();
        }
        cout << "Choose where to place the GEESE." << endl;
        unsigned newGeeseLocation = geeseLocation;
        while (newGeeseLocation == geeseLocation || newGeeseLocation > 18) {
            // if EOF detected, return false
            if (!(cin >> newGeeseLocation) && cin.eof()) {
                return false;
            }
        }
        // update textdisplay
        textDisplay.removeGeese(geeseLocation);
        textDisplay.setGeese(newGeeseLocation);
        geeseLocation = newGeeseLocation;

        auto tile = tiles[newGeeseLocation];
        unordered_map<int, int> buildersOnTile = getBuildersFromTile(tile.get()->getNumber());
        bool printMsg = false;
        buildersOnTile.erase(curTurn);
        for (auto const &tuple : buildersOnTile) {
            int b, bp;
            tie(b, bp) = tuple;
            printMsg = true;
            cout << "Builder " << builder->getColour() << " can choose to steal from "
                 << builders[b].get()->getColour() << endl;
        }

        if (printMsg) {
            cout << "Choose a builder to steal from." << endl;
            bool askForInput = true;
            while (askForInput) {
                string input;
                if (!(cin >> input) && cin.eof()) {
                    return false;
                }
                if (!input.empty()) {
                    toupper(input[0]);  // capitalize first letter
                    for (auto const &tuple : buildersOnTile) {
                        int b, bp;
                        tie(b, bp) = tuple;
                        auto tempBuilder = builders[b].get();
                        if (input[0] == tempBuilder->getColour()[0]) {
                            // TODO: check if correct
                            int stolenResource = tempBuilder->tryStealing();
                            if (stolenResource >= 0) {
                                cout << "Builder " << builder->getColour() << " steals "
                                     << getResourceName(stolenResource)
                                     << " from " << tempBuilder->getColour() << endl;
                                builder->setResource(stolenResource, builder->getResource(stolenResource) + 1);
                            }
                            askForInput = false;
                            break;
                        }
                    }
                }
            }
        } else {
            cout << "Builder " << builder->getColour() << " has no builders to steal from" << endl;
        }

    } else {
        // use num to give builders resources
        vector<shared_ptr<Tile>> tilesWithValue;
        // keep track of whether or not resources were gained given the dice roll
        bool gainedResources = false;
        for (size_t i = 0; i < tiles.size(); i++) {
            auto tile = tiles[i];
            // if tile has the same value as the dice...
            if (tile.get()->getValue() == diceVal) {
                int resourceCode = tile.get()->getResource();
                unordered_map<int, int> buildersOnTile = getBuildersFromTile(tile.get()->getNumber());
                for (auto const &tuple : buildersOnTile) {
                    gainedResources = true;
                    int b, bp;
                    tie(b, bp) = tuple;
                    // add resources based on number of improvements on tile
                    auto temp = builders[b].get();
                    temp->setResource(resourceCode, temp->getResource(resourceCode) + bp);
                    cout << "Builder " << temp->getColour() << " gained:" << endl
                         << bp << getResourceName(resourceCode);
                }
                // distribute resources for all vertices and shit... you know the drill
                // get building points per builder in a map for the tile
            }
        }
        if (!gainedResources) {
            cout << "No builders gained resources." << endl;
        }
    }

    // post-roll menu for current player
    while (true) {
        string temp;
        cout << "Enter a command." << endl
             << "> ";
        if (!(cin >> temp) && cin.eof()) {
            return false;
        } else if (temp == "help" || temp == "h") {
            cout << "Valid commands:" << endl;
            cout << "~ board                        : prints the current board" << endl;
            cout << "~ status                       : prints the current status of all builders ini order from builder 0 to 3" << endl;
            cout << "~ residences                   : prints the residences the current buildere has completed" << endl;
            cout << "~ build-road <road#>           : attempts to build a road at <road#>" << endl;
            cout << "~ build-res <housing#>         : attempts to build a basement at <housing#>" << endl;
            cout << "~ improve <housing#>           : attempts to improve the residence at <housing#>" << endl;
            cout << "~ trade <colour> <give> <take> : attempts to trade with builder <colour> giving one resource of type <give> and receving one resource of type <take>" << endl;
            cout << "~ next                         : passes control onto the next builder in the game. This ends the \"During the Turn\" phase." << endl;
            cout << "~ save <file>                  : saves the current game state to <file>" << endl;
            cout << "~ help                         : prints out the list of commands." << endl;
        } else if (temp == "board" || temp == "print") {
            printBoard();
        } else if (temp == "status") {
            printStatus();
        } else if (temp == "residences") {
            // TODO test against sample executable
            if (builder->getBuildingPoints()) {
                bool firstPrint = true;
                for (size_t i = 0; i < resLocations.size(); i++) {
                    // print out each location if vertex owner = curBuilder
                    if (vertices[resLocations[i]].get()->getOwner() == curTurn) {
                        if (!firstPrint) {
                            cout << ", ";
                            firstPrint = false;
                        }
                        cout << i;
                    }
                    cout << endl;
                }
            }
        } else if (temp == "build-road" || temp == "brd") {
            int edgeLocation;
            while (true) {
                if (cin >> edgeLocation) {
                    break;
                } else if (cin.eof()) {
                    return false;
                }
            }
            //  check if edge is valid, resources, etc...
            if (edgeLocation >= 0 && edgeLocation <= 71 && isValidEdge(edges[edgeLocation])) {
                if (edges[edgeLocation].get()->buildRoad(builderShared)) {
                    cout << "Builder " << builder->getColour()
                         << " built a road at " << edgeLocation << endl;
                    textDisplay.buildRoad(edges[edgeLocation], builderShared);
                } else {
                    cout << "You do not have enough resources." << endl;
                }
            } else {
                cout << "You cannot build here" << endl;
            }
        } else if (temp == "build-res" || temp == "brs") {
            int vertexLocation;
            while (true) {
                if (cin >> vertexLocation) {
                    break;
                } else if (cin.eof()) {
                    return false;
                }
            }
            //  check if vertex is valid, resources, etc...
            if (vertexLocation >= 0 && vertexLocation <= 53 && isValidVertex(vertices[vertexLocation])) {
                if (vertices[vertexLocation].get()->upgradeResidence(builderShared)) {
                    cout << "Builder " << builder->getColour()
                         << " built a basement at " << vertexLocation << endl;
                    textDisplay.updateVertex(vertices[vertexLocation], builderShared);
                } else {
                    cout << "You do not have enough resources." << endl;
                }
            } else {
                cout << "You cannot build here" << endl;
            }
        } else if (temp == "improve" || temp == "i") {
            int vertexLocation;
            while (true) {
                if (cin >> vertexLocation) {
                    break;
                } else if (cin.eof()) {
                    return false;
                }
            }
            if (vertexLocation >= 0 && vertexLocation <= 53) {
                auto vertex = vertices[vertexLocation];
                // if vertex has a different owner or is a tower
                if (!vertex.get()->canUpgrade(builderShared)) {
                    cout << "You cannot build here." << endl;
                } else if (vertex.get()->upgradeResidence(builderShared)) {
                    cout << "Residence upgrade succesful" << endl;
                    textDisplay.updateVertex(vertex, builderShared);
                    if (builder->hasWon()) {
                        cout << "Builder " << builder->getColour() << " has won the game!!" << endl;
                    }
                } else {
                    cout << "You do not have enough resources." << endl;
                }
            }

        } else if (temp == "trade") {
            Colour colour2;
            Resource resGive;
            Resource resTake;
            unordered_map<char, int> colours;
            // add colours to a map
            for (int i = 0; i < NUM_BUILDERS; i++) {
                if (i != curTurn) {
                    colours[builders[i].get()->getColour()[0]] = i;
                }
            }
            if ((cin >> colour2) && (cin >> resGive) && !(cin >> resTake)) {
                if (colour2 != curTurn) {
                    cout << "You cannot trade with yourself!" << endl;
                    // if trade function fails, return false
                } else if (resGive == resTake) {
                    cout << "You cannot trade the same resource!" << endl;
                } else {
                    if (!tradeWith(builders[colour2], resGive, resTake)) {
                        return false;
                    }
                }
            } else if (cin.eof()) {
                return false;
            }
        } else if (temp == "next") {
            break;
        } else if (temp == "save") {
            string filename;
            if (!(cin >> filename) && cin.eof()) {
                return false;
            }
            saveGame(filename);
        } else {
            cout << "Invalid command." << endl;
        }
    }

    ++curTurn;
    return true;
}

bool Game::tradeWith(shared_ptr<Builder> &builder, Resource resGive, Resource resTake) {
    auto curBuilder = builders[curTurn].get();
    auto otherBuilder = builder.get();
    if (curBuilder->getResource(resGive)) {
        cout << "You do not have "
             << "(" << getResourceName(resGive) << ")" << endl;
    } else if (!otherBuilder->getResource(resTake)) {
        cout << otherBuilder->getColour() << "does not have " << getResourceName(resTake) << endl;
    } else {
        cout << curBuilder->getColour() << "offers" << otherBuilder->getColour() << " one " << getResourceName(resGive)
             << " for one " << getResourceName(resTake) << "." << endl
             << "Does " << otherBuilder->getColour() << " accept this offer?";
        string yesOrNo;
        if (cin >> yesOrNo) {
            if (yesOrNo[0] == 'y' || yesOrNo[0] == 'Y') {
                curBuilder->setResource(resTake, curBuilder->getResource(resTake) + 1);
                curBuilder->setResource(resGive, curBuilder->getResource(resGive) - 1);

                otherBuilder->setResource(resTake, otherBuilder->getResource(resTake) - 1);
                otherBuilder->setResource(resGive, otherBuilder->getResource(resGive) + 1);
                // TODO: maybe make more informative
                cout << "Trade successful." << endl;
            } else {
                cout << "Trade offer was declined." << endl;
            }
        }
        if (cin.eof()) {
            return false;
        }
    }
    return true;
}

// TODO: bonus feature
void Game::marketTrade(Resource resource1, Resource resource2) {}

void Game::resetGame() {
    if (geeseLocation < 19) {
        textDisplay.removeGeese(geeseLocation);
    }

    for (size_t i = 0; i < 4; i++) {
        builders[i].get()->reset();
    }
    for (size_t i = 0; i < vertices.size(); i++) {
        auto vertex = vertices[i].get();
        vertex->reset();
        textDisplay.setInt(vertex->getCol(), vertex->getRow(), vertex->getNum());
    }

    for (size_t i = 0; i < edges.size(); i++) {
        auto edge = edges[i].get();
        edge->reset();
        textDisplay.setInt(edge->getRow(), edge->getCol(), edge->getNum());
    }

    resLocations.clear();
    roadLocations.clear();

    curTurn = -1;
    geeseLocation = -1;
    gameStarted = false;
    gameOver = false;
}
