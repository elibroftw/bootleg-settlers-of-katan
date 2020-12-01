#include "game.h"
#include "builder.h"

Game::Game() builders{}, curBuilder{0}, tiles{}, vertices{}, edges{}, textDisplay{}
{
    // make a constructor with optional layout and optional import
    for (unsigned_int i = 0; i < 4; i++)
    {
        // auto builder = std::make_shared<Builder>();
    }

    // vector<shared_pointer<Tile>> tiles;
    // vector<shared_pointer<Vertex>> vertices;
    // vector<shared_pointer<Edge>> edges;
}

void Game::createBoard()
{
    // check if layout.txt exists
}

void Game::createBoard(string filename)
{
    // read from file
}
void Game::moveGeese(int to)
{
}

void Game::saveGame(string filename)
{
}

void Game::loadGame(string filename)
{
}

void Game::newGame()
{
}

void Game::printBoard()
{
}

void Game::nextTurn() {}
void Game::tradeWith(Builder &builder, Resource resource1, Resource resource2) {}
void Game::endGame() {}
void Game::stealFrom(Builder &builder, Resource resource) {}
bool Game::isGameOver() {}
void Game::marketTrade(Resource resource1, Resource resource2) {}
