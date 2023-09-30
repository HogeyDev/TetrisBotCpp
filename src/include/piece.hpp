#include "types.hpp"
#include <iostream>
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

class Piece {
  public:
    int rotationState;
    int y;
    int x;
    int frames;
    int pieceId;

    Piece(int pieceId);

    Piece* clone();
    std::string getPrintableWithWhitespace();
    char getName();
    PieceMatrix getMatrix();
    void rotate(int rotationDirection);
    int getRotationStates();
    int getColorScheme();
};