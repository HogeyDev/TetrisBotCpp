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

Piece* Piece::clone() {
    Piece* copy = new Piece(this->pieceId);
    copy->rotationState = this->rotationState;
    copy->y = this->y;
    copy->x = this->x;
    copy->frames = this->frames;
    return copy;
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
    for (unsigned int y = 0; y < pieceMatrix.size(); y++) {
        for (unsigned int x = 0; x < pieceMatrix[y].size(); x++) {
            ret += pieceMatrix[y][x] ? "[]" : "  ";
        }
        if (y < pieceMatrix.size() - 1)
            ret += '\n';
    }
    if (pieceMatrix.size() == 3)
        ret += "\n";
    return ret;
}

PieceMatrix Piece::getMatrix() {
    switch (this->pieceId) {
    case 0:
        switch (this->rotationState) {
        case 0:
            return {
                {0, 0, 0},
                {1, 1, 1},
                {1, 0, 0},
            };
        case 1:
            return {
                {1, 1, 0},
                {0, 1, 0},
                {0, 1, 0},
            };
        case 2:
            return {
                {0, 0, 1},
                {1, 1, 1},
                {0, 0, 0},
            };
        case 3:
            return {
                {0, 1, 0},
                {0, 1, 0},
                {0, 1, 1},
            };
        }
    case 1:
        switch (this->rotationState) {

        case 0:
            return {
                {0, 0, 0},
                {1, 1, 1},
                {0, 0, 1},
            };
        case 1:

            return {
                {0, 1, 0},
                {0, 1, 0},
                {1, 1, 0},
            };
        case 2:
            return {
                {1, 0, 0},
                {1, 1, 1},
                {0, 0, 0},
            };
        case 3:
            return {
                {0, 1, 1},
                {0, 1, 0},
                {0, 1, 0},
            };
        }
    case 2:
        switch (this->rotationState) {
        case 0:
            return {
                {0, 0, 0, 0},
                {0, 1, 1, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 0},
            };
        }
    case 3:
        switch (this->rotationState) {
        case 0:
            return {
                {0, 0, 0},
                {0, 1, 1},
                {1, 1, 0},
            };
        case 1:
            return {
                {0, 1, 0},
                {0, 1, 1},
                {0, 0, 1},
            };
        }
    case 4:
        switch (this->rotationState) {
        case 0:
            return {
                {0, 0, 0},
                {1, 1, 0},
                {0, 1, 1},
            };
        case 1:
            return {
                {0, 0, 1},
                {0, 1, 1},
                {0, 1, 0},
            };
        }
    case 5:
        switch (this->rotationState) {
        case 0:
            return {
                {0, 0, 0},
                {1, 1, 1},
                {0, 1, 0},
            };
        case 1:
            return {
                {0, 1, 0},
                {1, 1, 0},
                {0, 1, 0},
            };
        case 2:
            return {
                {0, 1, 0},
                {1, 1, 1},
                {0, 0, 0},
            };
        case 3:
            return {
                {0, 1, 0},
                {0, 1, 1},
                {0, 1, 0},
            };
        }
    case 6:
        switch (this->rotationState) {
        case 0:
            return {
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0},
            };
        case 1:
            return {
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0},
            };
        }
    default: {
        std::cerr << "No Matching Piece Matrix For: (pieceId: " << this->pieceId
                  << ", rotationState: " << this->rotationState << ")"
                  << std::endl;
        exit(1);
    }
    }
}

void Piece::rotate(int rotationDirection) {
    this->rotationState = (this->rotationState + rotationDirection + 4) %
                          this->getRotationStates();
}

int Piece::getRotationStates() {
    static const int values[] = {4, 4, 1, 2, 2, 4, 2};
    return values[this->pieceId];
}

int Piece::getColorScheme() {
    static const int values[] = {2, 3, 1, 3, 2, 1, 1};
    return values[this->pieceId];
}