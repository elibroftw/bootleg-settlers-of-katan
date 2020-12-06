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
using std::endl;
using std::ifstream;
using std::invalid_argument;
using std::locale;
using std::make_pair;
using std::ofstream;
using std::pair;
using std::string;
using std::tie;
using std::toupper;
using std::unordered_map;
using std::unordered_set;

Game::Game() : curTurn{-1}, geeseLocation{19}, gameStarted{false}, gameOver{false} {
    // create builders
    builders.push_back(std::make_shared<Builder>("Blue"));
    builders.push_back(std::make_shared<Builder>("Red"));
    builders.push_back(std::make_shared<Builder>("Orange"));
    builders.push_back(std::make_shared<Builder>("Yellow"));

    verticesMap.resize(VM_HEIGHT);
    for (size_t row = 0; row < verticesMap.size(); row++) {
        verticesMap[row].resize(VM_WIDTH);
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

    edgesMap.resize(EM_HEIGHT);
    for (size_t row = 0; row < edgesMap.size(); row++) {
        // might need a Vertex default ctor (-1, -1, -1)
        edgesMap[row].resize(EM_WIDTH);
    }

    // create 71 edges
    // edges can be vertical or horizontal
    for (int i = 0; i < 71; i++) {
        int xCoord = 0;
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
        } else {
            cerr << "SOMETHING WENT WRONG" << endl;
        }

        int yCoord = 0;
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
        } else {
            cerr << "SOMETHING WENT WRONG SETTING yCoord" << endl;
        }
        auto edge = std::make_shared<Edge>(i, xCoord, yCoord, isHorizontal);
        edges.push_back(edge);
        edgesMap[xCoord / 5][yCoord / 2] = edge;
    }
}

pair<int, int> Game::getVertexFromCoords(int x, int y) {
    if ((x - 2) % 10 != 0 || y % 4 != 0) {
        throw InvalidArgument();
    }
    return make_pair(x / 10, y / 4);
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
    // for (size_t i = 0; i < 18; i++) {
    //     auto tile = std::make_shared<Tile>(i);
    //     tiles.push_back(tile);
    // }
}

void Game::createBoard(string filename) {
    ifstream file{filename};
    if (!file) {
        // if file exists or is invalid, create a random board
        createBoard();
        return;
    }
    unsigned int resource;
    unsigned int value;
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
        outfile << builders[i].get() << " r";
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
                    throw InvalidSaveFile();
                }
            }
        }

        // read roads
        while (true) {
            ss1 >> temp;
            if (temp == "h") {
                break;
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
                resLocations.push_back(vertexIdx);
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
            textDisplay.setTileResource(tiles.size(), getResourceName(resource));
            textDisplay.setTileValue(tiles.size(), value);
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
    // check if it and adjacent verticies in the map have no owners
    // TODO: fix since some are not adjacent
    for (int y = yCoord - 1; y < yCoord + 1; y++) {
        // check if vertex above, below and itself have owners
        if (y >= 0 && y <= VM_HEIGHT && verticesMap[y][xCoord].get()->getOwner() != -1) {
            // vertex is invalid since it or an vertex above/below has an owner
            return false;
        }
    }
    int x;
    int vertexNum = vertex.get()->getNum();
    bool leftIsFlat = false;
    if ((vertexNum >= 13 && vertexNum <= 16) ||
        (vertexNum >= 25 && vertexNum <= 28) || (vertexNum >= 37 && vertexNum <= 40)) {
        x = xCoord + (vertexNum % 2 ? 1 : -1);
        leftIsFlat = vertexNum % 2 ? true : false;
    } else {
        x = xCoord + (vertexNum % 2 ? -1 : 1);
        leftIsFlat = vertexNum % 2 ? false : true;
    }
    if (x >= 0 && x <= VM_WIDTH) {
        if (verticesMap[yCoord][x].get()->getOwner() != -1) {
            return false;
        }
    }

    // look for adjacent roads

    if (considerEdges) {
        int edgeX, edgeY;
        // check +- 2 yCoord edges
        if (yCoord + 2 < TD_HEIGHT) {
            tie(edgeX, edgeY) = getEdgeFromCoords(xCoord, yCoord + 2);
            if (edgesMap[edgeX][edgeY].get()->getOwner() == curTurn) return true;
        }

        if (yCoord - 2 >= 0) {
            tie(edgeX, edgeY) = getEdgeFromCoords(xCoord, yCoord - 2);
            if (edgesMap[edgeY][edgeY].get()->getOwner() == curTurn) return true;
        }

        if (leftIsFlat) { // check +1 xCoord if left is flat
            tie(edgeX, edgeY) = getEdgeFromCoords(xCoord + 5, yCoord);
            if (edgesMap[edgeY][edgeY].get()->getOwner() == curTurn) return true;
        } else {  // check -1 xCoord if right is flat
            tie(edgeX, edgeY) = getEdgeFromCoords(xCoord - 5, yCoord);
            if (edgesMap[edgeY][edgeY].get()->getOwner() == curTurn) return true;
        }
        return false;
    }
    return true;

}

bool Game::isValidEdge(shared_ptr<Edge> edge) {
    int xCoord = edge.get()->getX();
    int xIdx = xCoord / 10;
    int yCoord = edge.get()->getY();
    int yIdx = yCoord / 4;
    // can't build on a built road
    if (edge.get()->getOwner() != -1) {
        return false;
    }
    for (int i = xIdx - 1; i < xIdx + 1; i++) {
        for (int j = yIdx - 1; j < yIdx + 1; j++) {
            if (i >= 0 && j >= 0 && i <= EM_HEIGHT && j <= EM_WIDTH) {
                if (edgesMap[i][j].get()->getOwner() == curTurn) {
                    // edge is valid since an adjacent edge is a road
                    return true;
                }
            }
        }
    }

    int vertexX, vertexY;
    // check +- 5 xCoord vertices if horizontal edge
    if (edge.get()->getHorizontal()) {
        tie(vertexX, vertexY) = getVertexFromCoords(xCoord - 5, yCoord);
        if (verticesMap[vertexX][vertexY].get()->getOwner() == curTurn) return true;

        tie(vertexX, vertexY) = getVertexFromCoords(xCoord + 5, yCoord);
        if (verticesMap[vertexX][vertexY].get()->getOwner() == curTurn) return true;

    } else {
        // check +- 2 yCoord vertices if vertical edge
        tie(vertexX, vertexY) = getVertexFromCoords(xCoord, yCoord - 2);
        if (verticesMap[vertexX][vertexY].get()->getOwner() == curTurn) return true;

        tie(vertexX, vertexY) = getVertexFromCoords(xCoord, yCoord + 2);
        if (verticesMap[vertexX][vertexY].get()->getOwner() == curTurn) return true;
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
                            for (size_t x = 0; x < resLocations.size(); x++) {
                                cout << resLocations[x] << " ";
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
    auto coords = textDisplay.getTopLeftCoord(tileNumber);
    int xCoord = coords.first + 1;
    int yCoord = coords.second;
    unordered_map<int, int> buildersOnTile;
    for (size_t x = 0; x < 2; x++) {
        for (size_t y = 0; y < 3; y++) {
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

void Game::printStatus() {
    for (size_t i = 0; i < 4; i++) {
        auto b = builders[i].get();
        string colour = b->getColour();
        unsigned int padding = 6 - colour.size();
        cout << "Builder " << colour << " ";
        for (size_t x = 0; x < padding; x++) {
            cout << " ";
        }
        cout << "has " << b->getBuilderPoints() << " building points";
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
    unsigned int diceVal = dice.roll(builder->isDiceLoaded());
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
        unsigned int newGeeseLocation = geeseLocation;
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
            locale loc;
            bool askForInput = true;
            while (askForInput) {
                string input;
                if (!(cin >> input) && cin.eof()) {
                    return false;
                }

                toupper(input[0], loc);  // capitalize first letter
                for (auto const &tuple : buildersOnTile) {
                    int b, bp;
                    tie(b, bp) = tuple;
                    auto tempBuilder = builders[b].get();
                    if (input[0] == tempBuilder->getColour()[0]) {
                        // TODO: check if correct
                        int stolenResource = tempBuilder->tryStealing();
                        if (stolenResource >= 0) {
                            cout << "Builder " << builder->getColour() << " steals "
                                 << getResourceName(stolenResource) << " from " << tempBuilder->getColour() << endl;
                            builder->setResource(stolenResource, builder->getResource(stolenResource) + 1);
                        }
                        askForInput = false;
                        break;
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
            // TODO test against sampel executaable
            if (builder->getBuilderPoints()) {
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
            // TODO: check if input location was valid,
            //  check if edge is valid, resources, etc...
            int edgeLocation;
            while (true) {
                if (cin >> edgeLocation) {
                    break;
                } else if (cin.eof()) {
                    return false;
                }
            }
            // cout << "You do not have enough resources." << endl;
            if (edgeLocation >= 0 && edgeLocation <= 71 && isValidEdge(edges[edgeLocation])) {
                if (edges[edgeLocation].get()->buildRoad(builderShared)) {
                    cout << "Builder " << builder->getColour()
                         << " built a road at " << edgeLocation << endl;
                } else {
                    cout << "You do not have enough resources." << endl;
                }
            } else {
                cout << "You cannot build here" << endl;
            }
        } else if (temp == "build-res" || temp == "brs") {
            // TODO: check if input location was valid,
            //  check if vertex is valid, resources, etc...
            int vertexLocation;
            while (true) {
                if (cin >> vertexLocation) {
                    break;
                } else if (cin.eof()) {
                    return false;
                }
            }
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
            // TODO: check if input location was valid,
            //  check if owner owns vertex, resources, etc...
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
                if (vertex.get()->getOwner() != curTurn || vertex.get()->getImprovement() == 'T') {
                    cout << "You cannot build here." << endl;
                } else if (vertex.get()->upgradeResidence(builderShared)) {
                    cout << "Residence upgrade succesful" << endl;
                    textDisplay.updateVertex(vertex, builderShared);
                    if (builder->getBuilderPoints() >= 10) {
                        cout << "Builder " << builder->getColour() << " has won the game!!" << endl;
                    }
                } else {
                    cout << "You do not have enough resources." << endl;
                }
            }

        } else if (temp == "trade") {
            string colour2;
            string resGive;
            string resTake;
            if ((cin >> colour2) && (cin >> resGive) && !(cin >> resTake)) {
                // TODO: validate inputs
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

void Game::tradeWith(Builder &builder, Resource resource1, Resource resource2) {
}

void Game::marketTrade(Resource resource1, Resource resource2) {}

// might be able to delete
void Game::stealFrom(Builder &builder, Resource resource) {}

void Game::resetGame() {
    curTurn = -1;
    if (geeseLocation < 19) {
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
