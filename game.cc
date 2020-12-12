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

using std::cerr;
using std::cin;
using std::cout;
using std::default_random_engine;
using std::endl;
using std::ifstream;
using std::invalid_argument;
using std::istringstream;
using std::make_pair;
using std::make_shared;
using std::ofstream;
using std::pair;
using std::random_shuffle;
using std::stoi;
using std::string;
using std::tie;
using std::toupper;
using std::uniform_int_distribution;
using std::unordered_map;
using std::unordered_set;

Game::Game() : curTurn{-1},
               geeseLocation{19},
               gameStarted{false},
               gameOver{false},
               verticesMap(VM_HEIGHT, vector<shared_ptr<Vertex>>(VM_WIDTH, make_shared<Vertex>())),
               edgesMap(EM_HEIGHT, vector<shared_ptr<Edge>>(EM_WIDTH, make_shared<Edge>())) {
    // create builders
    builders.push_back(make_shared<Builder>("Blue", 0));
    builders.push_back(make_shared<Builder>("Red", 1));
    builders.push_back(make_shared<Builder>("Orange", 2));
    builders.push_back(make_shared<Builder>("Yellow", 3));

    // initialize maps
    // verticesMap = vector<vector<shared_ptr<Vertex>>>(VM_HEIGHT, vector<shared_ptr<Vertex>>(VM_WIDTH, make_shared<Vertex>()));
    // edgesMap = vector<vector<shared_ptr<Edge>>>(EM_HEIGHT, vector<shared_ptr<Edge>>(EM_WIDTH, make_shared<Edge>()));

    // create 53 vertices
    for (int i = 0; i <= 53; i++) {
        auto vertex = make_shared<Vertex>(i);
        int rowTD = vertex.get()->getRow();
        int colTD = vertex.get()->getCol();
        vertices.push_back(vertex);
        int vertexR, vertexC;
        tie(vertexR, vertexC) = Vertex::getVertexFromCoords(rowTD, colTD);
        verticesMap.at(vertexR).at(vertexC) = vertex;
        textDisplay.setInt(rowTD, colTD, i);
    }

    // edgesMap.reserve(EM_HEIGHT);
    // for (size_t r = 0; r < EM_HEIGHT; r++) {
    //     vector<shared_ptr<Edge>> row;
    //     for (size_t c = 0; c < EM_WIDTH; c++) {
    //         row.push_back(make_shared<Edge>());
    //     }
    //     edgesMap.push_back(row);
    // }

    // create 71 edges
    // edges can be vertical or horizontal
    for (int i = 0; i <= 71; i++) {
        auto edge = make_shared<Edge>(i);
        int rowTD = edge.get()->getRow();
        int colTD = edge.get()->getCol();
        edges.push_back(edge);
        int edgeR, edgeC;
        tie(edgeR, edgeC) = Edge::getEdgeFromCoords(rowTD, colTD);
        edgesMap.at(edgeR).at(edgeC) = edge;
        textDisplay.setInt(rowTD, colTD, i);
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

    vector<unsigned> randomValues;
    for (size_t i = 0; i < 3; i++) {
        randomValues.push_back(2);
        randomValues.push_back(distrib1(rng));
        randomValues.push_back(distrib1(rng));
        randomValues.push_back(distrib2(rng));
        randomValues.push_back(distrib2(rng));
        randomValues.push_back(12);
    }

    vector<Resource> randomResources = {Wifi, Wifi, Wifi, Heat, Heat, Heat,
                                        Brick, Brick, Brick, Brick,
                                        Energy, Energy, Energy, Energy,
                                        Glass, Glass, Glass, Glass};

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
        textDisplay.updateTile(tile);
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
            cout << "ERROR: layout.txt was not formatted correctly" << endl;
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
            roads[owner] = vector<int>{i};
        }
    }

    unordered_map<int, vector<int>> housing;
    for (auto &&i : resLocations) {
        int owner = vertices[i].get()->getOwner();
        if (housing.count(owner)) {
            housing[owner].push_back(i);
        } else {
            housing[owner] = vector<int>{i};
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
        outfile << tile->getResource() << " " << tile->getValue();
    }
    outfile << endl;
    if (geeseLocation < 19) {
        outfile << geeseLocation << endl;
    }
    cout << "Saved game to " << filename << endl;
}

void Game::loadGame(string filename) {
    ifstream file{filename};
    if (!file) {
        cout << "ERROR: Save File Does Not Exist" << endl;
        throw InvalidSaveFile();
    }
    file >> curTurn;
    file.ignore(1000, ' ');
    string line;
    // read builder data for each buildere
    for (size_t i = 0; i < 4; i++) {
        // for each builder
        if (!getline(file, line)) {
            cout << "ERROR: Invalid Save File (builder data)" << endl;
            throw InvalidSaveFile();
        }
        auto builder = builders[i];
        istringstream ss1{line};
        string temp;
        // read resources
        for (size_t i = 0; i < 5; i++) {
            // read from sstream
            if (!(ss1 >> temp)) {
                cout << "ERROR: Invalid Save File (builder data - resources empty)" << endl;
                throw InvalidSaveFile();
            }
            if (temp == "r") {
                break;
            } else {
                // if read was valid, try converting value to int
                try {
                    builder.get()->setResource(i, stoi(temp));
                } catch (invalid_argument &e) {
                    cout << "ERROR: Invalid Save File (builder data - resources)" << endl;
                    throw InvalidSaveFile();
                }
            }
        }

        // read roads
        while (true) {
            if (!(ss1 >> temp)) {
                cout << "ERROR: Invalid Save File (builder data - roads - empty)" << endl;
                throw InvalidSaveFile();
            }
            if (temp == "h") {
                break;
            } else if (temp != "r") {
                try {
                    int roadIdx = stoi(temp);
                    auto edge = edges.at(roadIdx);
                    edge.get()->buildRoad(builder, false);
                    textDisplay.buildRoad(edge, builder);
                    roadLocations.push_back(roadIdx);
                } catch (invalid_argument &e) {
                    cout << "ERROR: Invalid Save File (builder data - roads), got " << temp << endl;
                    throw InvalidSaveFile();
                }
            }
        }

        // read housing
        int vertexIdx;
        while (ss1 >> vertexIdx) {
            if (ss1 >> temp) {
                // call addHousing to Vertex
                auto vertex = vertices.at(vertexIdx);
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
                cout << "ERROR: Invalid Save File (builder data - housing)" << endl;
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
    istringstream ss2{line};
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
    cout << "GAME LOADED" << endl;
    for (auto &&b : builders) {
        if (b.get()->hasWon()) {
            b.get()->winGame();
            gameOver = true;
        }
    }
}

bool Game::isValidVertex(shared_ptr<Vertex> vertex, bool considerEdges) {
    int rowTD = vertex.get()->getRow();
    int row = rowTD / 4;
    int colTD = vertex.get()->getCol();
    int col = colTD / 10;

    // check if it and adjacent verticies in the map have no owners
    for (int r = row - 1; r < row + 1; r++) {
        if (r >= 0 && r < VM_HEIGHT && verticesMap.at(r).at(col).get()->getOwner() != -1) {
            // vertex is invalid since the vertex above, below or itself has an owner
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
    if (c >= 0 && c < VM_WIDTH) {
        if (verticesMap.at(row).at(c).get()->getOwner() != -1) {
            return false;
        }
    }
    // look for adjacent roads
    if (considerEdges) {
        int edgeR, edgeC;

        if (rowTD - 2 >= 0) {
            // check if edge above is a road
            tie(edgeR, edgeC) = Edge::getEdgeFromCoords(rowTD - 2, colTD);
            if (edgesMap.at(edgeR).at(edgeC).get()->getOwner() == curTurn) return true;
        }

        if (rowTD + 2 < TD_HEIGHT) {
            // check if edge below is a road
            tie(edgeR, edgeC) = Edge::getEdgeFromCoords(rowTD + 2, colTD);
            if (edgesMap.at(edgeR).at(edgeC).get()->getOwner() == curTurn) return true;
        }

        if (leftIsFlat) {
            // since left is flat, check if edge to the right is flat
            tie(edgeR, edgeC) = Edge::getEdgeFromCoords(rowTD, colTD + 5);
            if (edgesMap.at(edgeR).at(edgeC).get()->getOwner() == curTurn) return true;
        } else {
            // since right is falt, check if edge to the lefet is flat
            tie(edgeR, edgeC) = Edge::getEdgeFromCoords(rowTD, colTD - 5);
            if (edgesMap.at(edgeR).at(edgeC).get()->getOwner() == curTurn) return true;
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
            if (colTD + colDelta >= 0 && colTD + colDelta < VM_WIDTH) {
                tie(vertexR, vertexC) = Vertex::getVertexFromCoords(rowTD, colTD + colDelta);
                auto vertex = verticesMap.at(vertexR).at(vertexC).get();
                if (vertex->getOwner() == curTurn) return true;
                else if (vertex->getOwner() == -1) {
                    // check if edges up and below are valid, given vertex is not blocked
                    int c = col + colDelta / 5;
                    for (int r = row - 1; r <= row + 1; r += 2) {
                        if (r >= 0 && r < EM_HEIGHT) {
                            if (edgesMap.at(r).at(c).get()->getOwner() == curTurn) {
                                // edge is valid since an adjacent edge is a road
                                return true;
                            }
                        }
                    }
                }
            }
        }
    } else {
        // check the vertecies above and below of 2 units
        for (int rowDelta = -2; rowDelta <= 2; rowDelta += 4) {
            if (rowTD + rowDelta >= 0 && rowTD + rowDelta < VM_HEIGHT) {
                tie(vertexR, vertexC) = Vertex::getVertexFromCoords(rowTD + rowDelta, colTD);
                auto vertex = verticesMap.at(vertexR).at(vertexC).get();
                if (vertex->getOwner() == curTurn) return true;
                else if (vertex->getOwner() == -1) {
                    // check if edges above, and right are valid, given vertex is not blocked
                    int r = row + rowDelta;
                    if (r >= 0 && r < EM_HEIGHT) {
                        if (edgesMap.at(r).at(col).get()->getOwner() == curTurn) {
                            return true;
                        }
                    }
                    r -= rowDelta / 2;
                    if (r >= 0 && r < EM_HEIGHT) {
                        for (int c = row - 1; c <= row + 1; c += 2) {
                            if (c >= 0 && c < EM_WIDTH) {
                                if (edgesMap.at(r).at(c).get()->getOwner() == curTurn) {
                                    return true;
                                }
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
    for (int i = 0; i < 2; i++) {
        for (size_t j = 0; j < 4; j++) {
            bool validVertex = false;
            while (!validVertex) {
                // accounts for reverse order
                auto builder = i == 0 ? builders.at(j) : builders.at(3 - j);
                cout << "Builder " << builder.get()->getColour()
                     << ", where do you want to build a basement?" << endl
                     << "> ";
                if (cin >> vertexIdx) {
                    if (vertexIdx >= 0 && vertexIdx <= 53) {
                        auto vertex = vertices.at(vertexIdx);
                        // build the basement
                        validVertex = isValidVertex(vertex, false);
                        if (validVertex) {
                            vertex.get()->upgradeResidence(builder, false);
                            resLocations.push_back(vertexIdx);
                        } else {
                            cout << "You cannot build here." << endl;
                            cout << "Basements already exist as locations: ";
                            for (size_t k = 0; k < resLocations.size(); k++) {
                                cout << resLocations.at(k) << " ";
                            }
                            cout << endl;
                        }
                    }
                } else if (cin.eof()) {
                    saveGame("backup.sv");
                    return false;
                } else {
                    resetCin();
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

    unordered_map<int, int> buildersOnTile;
    for (size_t r = 0; r <= 8; r += 4) {
        for (size_t c = 0; c <= 10; c += 10) {
            int vertexR, vertexC;
            tie(vertexR, vertexC) = Vertex::getVertexFromCoords(row + r, col + 1 + c);
            auto vertex = verticesMap.at(vertexR).at(vertexC);
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
    for (auto &&b : builders) b.get()->printStatus();
}

void Game::resetCin() {
    cin.clear();
    cin.ignore(1000, '\n');
}

bool Game::nextTurn() {
    // whenever a builder builds, check if builder has 10+ points
    // if it does set gameEnded to true
    cout << "Builder " << builders.at(curTurn).get()->getColour() << "'s turn." << endl;

    // rolling the dice
    bool rollDice = false;
    auto builderShared = builders.at(curTurn);
    auto builder = builderShared.get();
    while (!rollDice) {
        string temp = " ";
        cout << "> ";
        if (!(cin >> temp)) {
            if (cin.eof()) {
                return false;
            }
            resetCin();
        }
        // TODO: switch to using lowered first letter
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
            cout << "Please enter 'help' for a list of valid commands." << endl;
        }
    }
    unsigned diceVal = dice.roll(builder->isDiceLoaded());
    // if EOF detected
    if (diceVal < 2 || diceVal > 12) {
        return false;
    }

    // geese
    if (diceVal == 7) {
        for (size_t i = 0; i < NUM_BUILDERS; i++) {
            builders.at(i).get()->geeseAttack();
        }
        cout << "Choose where to place the GEESE." << endl;
        unsigned newGeeseLocation = geeseLocation;
        while (newGeeseLocation == geeseLocation || newGeeseLocation > 18) {
            // if EOF detected, return false
            cout << "> ";
            if (!(cin >> newGeeseLocation)) {
                if (cin.eof()) {
                    return false;
                }
                resetCin();
            }
        }
        // update textdisplay
        textDisplay.removeGeese(geeseLocation);
        textDisplay.setGeese(newGeeseLocation);
        geeseLocation = newGeeseLocation;

        auto tile = tiles.at(newGeeseLocation);
        unordered_map<int, int> buildersOnTile = getBuildersFromTile(tile.get()->getNumber());
        bool printMsg = false;
        buildersOnTile.erase(curTurn);
        for (auto const &tuple : buildersOnTile) {
            int b, bp;
            tie(b, bp) = tuple;
            printMsg = true;
            cout << "Builder " << builder->getColour() << " can choose to steal from "
                 << builders.at(b).get()->getColour() << endl;
        }

        if (printMsg) {
            cout << "Choose a builder to steal from." << endl;
            bool askForInput = true;
            while (askForInput) {
                cout << "> ";
                string input;
                if (!(cin >> input)) {
                    if (cin.eof()) {
                        return false;
                    }
                    resetCin();
                }
                if (!input.empty()) {
                    input[0] = toupper(input[0]);  // capitalize first letter
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
        bool aBuilderGained = false;

        for (size_t i = 0; i < tiles.size(); i++) {
            auto tile = tiles.at(i);
            // if tile has the same value as the dice...

            if (tile.get()->getValue() == diceVal) {
                int resourceCode = tile.get()->getResource();
                unordered_map<int, int> buildersOnTile = getBuildersFromTile(i);
                for (auto const &tuple : buildersOnTile) {
                    aBuilderGained = true;
                    int b, resourcesGained;
                    tie(b, resourcesGained) = tuple;
                    // add resources based on number of improvements on tile
                    auto temp = builders.at(b).get();
                    temp->setResource(resourceCode, temp->getResource(resourceCode) + resourcesGained);
                    cout << "Builder " << temp->getColour() << " gained:" << endl
                         << resourcesGained << ' ' << getResourceName(resourceCode) << endl;
                }
                // distribute resources for all vertices and shit... you know the drill
                // get building points per builder in a map for the tile
            }
        }
        if (!aBuilderGained) {
            cout << "No builders gained resources." << endl;
        }
    }

    // post-roll menu for current player
    while (true) {
        string temp;
        cout << "Enter a command." << endl
             << "> ";
        if (!(cin >> temp)) {
            if (cin.eof()) {
                return false;
            }
            resetCin();

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
                cout << builder->getColour() << " has built:" << endl;
                for (size_t i = 0; i < resLocations.size(); i++) {
                    // prints out
                    auto vertex = vertices.at(resLocations.at(i)).get();
                    if (vertex->getOwner() == curTurn) {
                        cout << i << ' ' << vertex->getImprovement() << endl;
                    }
                }
            }
        } else if (temp == "build-road" || temp == "brd") {
            int edgeLocation;
            if (!(cin >> edgeLocation)) {
                if (cin.eof()) {
                    return false;
                }
                cout << "ERROR: input must be an integer." << endl
                     << "> ";
                resetCin();
            } else if (edgeLocation < 0 || edgeLocation > 71) {
                cout << "ERROR: integer must be between [0, 71]." << endl;
            } else if (!isValidEdge(edges.at(edgeLocation))) {
                cout << "You cannot build here" << endl;
            } else if (edges.at(edgeLocation).get()->buildRoad(builderShared)) {
                cout << "Builder " << builder->getColour()
                     << " built a road at " << edgeLocation << endl;
                textDisplay.buildRoad(edges.at(edgeLocation), builderShared);
            } else {
                cout << "You do not have enough resources." << endl
                     << "A Road costs 1 HEAT and 1 WIFI.";
            }
        } else if (temp == "build-res" || temp == "brs") {
            int vertexLocation;
            if (!(cin >> vertexLocation)) {
                if (cin.eof()) {
                    return false;
                }
                cout << "ERROR: input must be an integer." << endl
                     << "> build-res ";
                resetCin();
            } else if (vertexLocation < 0 || vertexLocation > 53) {
                cout << "ERROR: integer must be between [0, 53]." << endl;
            } else if (!isValidVertex(vertices.at(vertexLocation))) {
                cout << "You cannot build here." << endl;
            } else if (vertices.at(vertexLocation).get()->upgradeResidence(builderShared)) {
                cout << "Builder " << builder->getColour()
                     << " built a basement at " << vertexLocation << endl;
                textDisplay.updateVertex(vertices.at(vertexLocation), builderShared);
            } else {
                cout << "You do not have enough resources." << endl
                     << "A Basement costs 1 BRICK, 1 ENERGY, 1 GLASS, and 1 WIFI." << endl;
            }
        } else if (temp == "improve" || temp == "i") {
            int vertexLocation;
            if (!(cin >> vertexLocation)) {
                if (cin.eof()) {
                    return false;
                }
                cout << "EERROR: input must be an integer." << endl
                     << "> ";
                resetCin();
            } else if (vertexLocation < 0 && vertexLocation > 53) {
                cout << "ERROR: integer must be between [0, 53]." << endl;
            } else if (!vertices.at(vertexLocation).get()->canUpgrade(builderShared)) {
                // if vertex has a different owner or is a tower
                cout << "You cannot build here." << endl;
            } else if (vertices.at(vertexLocation).get()->upgradeResidence(builderShared)) {
                cout << "Residence upgrade succesful" << endl;
                textDisplay.updateVertex(vertices.at(vertexLocation), builderShared);
                if (builder->hasWon()) {
                    builder->winGame();
                    gameOver = true;
                    return true;
                }
            } else {
                cout << "You do not have enough resources." << endl
                     << "Improving a Basement to a House costs 2 GLASS and 3 HEAT." << endl
                     << "Improving a House to a Tower costs 3 BRICK, 2 ENERGY, 2 GLASS, 1 WIFI, and 2 HEAT." << endl;
            }

        } else if (temp == "trade") {
            Colour colour2;
            Resource resGive;
            Resource resTake;
            unordered_map<char, int> colours;
            // add colours to a map
            for (int i = 0; i < NUM_BUILDERS; i++) {
                if (i != curTurn) {
                    colours[builders.at(i).get()->getColour()[0]] = i;
                }
            }
            if ((cin >> colour2) && (cin >> resGive) && (cin >> resTake)) {
                cout << "jaslkdjasoidjlaksjdkljasjd" << endl;
                if (colour2 != curTurn) {
                    cout << "You cannot trade with yourself!" << endl;
                    // if trade function fails, return false
                } else if (resGive == resTake) {
                    cout << "You cannot trade the same resource!" << endl;
                } else {
                    if (!tradeWith(builders.at(colour2), resGive, resTake)) {
                        return false;
                    }
                }
            } else if (cin.eof()) {
                return false;
            } else {
                resetCin();
            }
        } else if (temp == "next") {
            break;
        } else if (temp == "save") {
            string filename;
            if (!(cin >> filename)) {
                if (cin.eof()) {
                    return false;
                }
                resetCin();
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
    auto curBuilder = builders.at(curTurn).get();
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
        } else if (cin.eof()) {
            return false;
        } else {
            resetCin();
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

void Game::test() {
    cout << "creating random board" << endl;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    createBoard(seed);

    cout << "testing print board" << endl;
    printBoard();

    cout << "testing Vertex" << endl;
    for (size_t r = 0; r < VM_HEIGHT; r++) {
        for (size_t c = 0; c < VM_WIDTH; c++) {
            auto vertex = verticesMap.at(r).at(c);
            vertex.get()->getImprovement();
            vertex.get()->getRow();
            vertex.get()->getCol();
            vertex.get()->getOwner();
            if (vertex.get()->getNum() != -1) {
                isValidVertex(vertex);
            }
        }
    }

    cout << "testing Edge" << endl;
    for (size_t r = 0; r < EM_HEIGHT; r++) {
        for (size_t c = 0; c < EM_WIDTH; c++) {
            auto edge = edgesMap.at(r).at(c);
            edge.get()->getOwner();
            if (edge.get()->getNum() != -1) {
                isValidEdge(edge);
            }
        }
    }

    cout << "testing getBuildersFromTile - no builds" << endl;
    for (size_t i = 0; i < tiles.size(); i++) {
        getBuildersFromTile(i);
    }
    cout << "testing getBuildersFromTile - some builds" << endl;

    for (size_t i = 0; i < vertices.size(); i += 10) {
        auto vertex = vertices.at(i);
        vertex.get()->upgradeResidence(builders[i % 4], false);
        resLocations.push_back(i);
    }
    for (size_t i = 0; i < tiles.size(); i++) {
        getBuildersFromTile(i);
    }

    cout << "testing printStatus" << endl;
    printStatus();
}
