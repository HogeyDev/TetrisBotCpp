#include "board.hpp"

class Game {
  public:
    int level;
    Board* board;

    Game(int startingLevel = 18);

    void tick();
};
