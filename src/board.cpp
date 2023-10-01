#include "board.hpp"
#include "types.hpp"
#include <iostream>

Board::Board() { this->boardState.resize(200); }

Board* Board::clone() {
    Board* copy = new Board();
    for (unsigned int i = 0; i < this->boardState.size(); i++) {
        copy->boardState[i] = this->boardState[i];
    }
    return copy;
}

int Board::setMinoXY(int value, int x, int y) {
    if (y < 0 || y > 19)
        return 0;
    if (x < 0 || x > 9)
        return 0;
    return (this->boardState.at(y * 10 + x) = value);
}

int Board::getMinoXY(int x, int y) {
    if (y < 0 || y > 19)
        return 0;
    if (x < 0 || x > 9)
        return 0;
    return this->boardState.at(y * 10 + x);
}

int Board::getMino(int offset) { return this->boardState[offset]; }

std::string Board::getString() {
    std::string ret = "";
    for (unsigned int i = 0; i < this->boardState.size(); i++) {
        if (i > 0 && i % 10 == 0)
            ret += "\n";
        ret += this->boardState[i] ? "[]" : "  ";
    }
    return ret;
}

void Board::addPieceToBoard(Piece* piece) {
    PieceMatrix pieceMatrix = piece->getMatrix();
    for (unsigned int y = 0; y < pieceMatrix.size(); y++) {
        for (unsigned int x = 0; x < pieceMatrix[y].size(); x++) {
            if (pieceMatrix[y][x]) {
                this->setMinoXY(pieceMatrix[y][x], piece->x + x, piece->y + y);
            }
        }
    }
}

void Board::removeRow(int row) {
    for (int x = 0; x < 10; x++) {
        this->boardState.erase(this->boardState.begin() + (10 * row + x));
    }
}
