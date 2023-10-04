#include "util.hpp"
#include "constants.hpp"
#include "move_search.hpp"
#include <cmath>
#include <cstdint>
#include <iostream>

int getFramesUntilPieceDrop(int level) {
    if (level >= 0 && level <= 8)
        return (48 - 5 * level);
    if (level == 9)
        return 6;
    if (level >= 9 && level <= 12)
        return 5;
    if (level >= 13 && level <= 15)
        return 4;
    if (level >= 16 && level <= 18)
        return 3;
    if (level >= 19 && level <= 28)
        return 2;
    if (level >= 29)
        return 1;
    std::cerr << "Unimplemented Level Gravity: " << level << std::endl;
    exit(1);
}

int sign(int number) {
    if (number < 0)
        return -1;
    if (number > 0)
        return 1;
    return 0;
}

int lastScareHeightHeight = 0;
int lastScareHeightLevel = -INT32_MAX;
int getScareHeight(int level) {
    if (lastScareHeightLevel == level)
        return lastScareHeightHeight;
    lastScareHeightHeight = std::floor(0.75 * getMaximumNTap(5, level));
    lastScareHeightLevel = level;
    return lastScareHeightHeight;
}

int getMaximumNTap(int numberOfTaps, int level) {
    bool maxHeightFound = false;
    int maxHeight = 0;
    while (!maxHeightFound && maxHeight < 20) {
        Game* testGame = new Game(level);
        testGame->activePiece = new Piece(6 /* "I" */);
        for (int i = 0; i < maxHeight; i++) {
            testGame->board->setMinoXY(1, 1, 19 - i);
        }
        testGame->simulatePiece(Move{-numberOfTaps, 1});
        if (!testGame->board->getMinoXY(0, 19)) {
            maxHeightFound = true;
            break;
        }
        maxHeight++;
    }
    return std::max(0, maxHeight - 1);
}
