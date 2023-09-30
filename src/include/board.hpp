#include "piece.hpp"
#include "types.hpp"
#include <string>

class Board {
  public:
    BoardState boardState;

    Board();

    Board* clone();
    int setMinoXY(int value, int x, int y);
    int getMinoXY(int x, int y);
    int getMino(int offset);
    std::string getString();
    void addPieceToBoard(Piece* piece);
};