#include "game.hpp"

Game::Game(int startingLevel) {
    this->level = startingLevel;
    this->board = new Board();
}