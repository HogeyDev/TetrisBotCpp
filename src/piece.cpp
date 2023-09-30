#include "piece.hpp"
#include "types.hpp"
#include <string>

/*
0 L
1 J
2 O
3 S
4 Z
5 T
6 I
*/

Piece::Piece(int pieceId) {
    this->pieceId = pieceId;
    this->rotationState = 0;
    this->y = this->getName() == 'I' ? -2 : -1;
    this->x = 3;
    this->frames = 0;
}

char Piece::getName() {
    switch (this->pieceId) {
    case 0:
        return 'L';
    case 1:
        return 'J';
    case 2:
        return 'O';
    case 3:
        return 'S';
    case 4:
        return 'Z';
    case 5:
        return 'T';
    case 6:
        return 'I';
    default: {
        std::cerr << "No Matching Piece ID: " << this->pieceId << std::endl;
        exit(1);
    }
    }
}

std::string Piece::getPrintableWithWhitespace() {
    PieceMatrix pieceMatrix = this->getMatrix();
    std::string ret;
    for (int y = 0; y < pieceMatrix.size(); y++) {
        for (int x = 0; x < pieceMatrix[y].size(); x++) {
            ret += pieceMatrix[y][x] ? "[]" : "  ";
        }
        if (y < pieceMatrix.size() - 1)
            ret += '\n';
    }
    return ret;
}

PieceMatrix Piece::getMatrix() {}
