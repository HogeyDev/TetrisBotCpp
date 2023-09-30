#include "board.hpp"
#include <iostream>

Board::Board() {
    this->boardState.resize(200);
    std::cout << this->boardState.size() << std::endl;
}
