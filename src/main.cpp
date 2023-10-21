#include "constants.hpp"
#include "move_search.hpp"
#include "rng.hpp"
#include "util.hpp"
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>

#define FRAMERATE 60

static const float frameMillis = (float)1000 / FRAMERATE;

int main() {
    srand(time(NULL));
    Game* game = new Game(startingLevel, rand());
    // Game *game = new Game(startingLevel, 1804289383);
    int oldPieceTotal = 0;

    // game->board->setMinoXY(1, 0, 19);
    // game->board->setMinoXY(1, 0, 18);
    // game->board->setMinoXY(1, 0, 17);
    // game->board->setMinoXY(1, 1, 19);
    // game->board->setMinoXY(1, 1, 17);
    // game->board->setMinoXY(1, 2, 19);
    // game->board->setMinoXY(1, 2, 17);
    // game->activePiece = new Piece(1);

    std::string inputTimeline = "";
    // std::string inputTimeline = "...F.............E";
    unsigned int i = -1;

    while (true) {
        if (oldPieceTotal != game->totalPieces) {
            inputTimeline = getBestMove(game, globalSearchDepth);
            i = 0;
            oldPieceTotal = game->totalPieces;
            // exit(0);
        }

        char movementCharacter = '.';
        if (i < inputTimeline.size())
            movementCharacter = inputTimeline[i];
        game->tick(movementCharacter);
        i++;

        int result = system("clear");
        (void)result;
        game->printPiecePreview();
        game->printBoard();
        //       std::cout << game->isOver << std::endl;
        std::cout << inputTimeline << std::endl;
        //       std::cout << evaluateGame(game) << std::endl;
        //       std::cout << getScareHeight(game->level) << std::endl;
        // std::cout << isTetrisReady(game) << std::endl;

        if (game->isOver) {
            std::cout << "GAME OVER!" << std::endl;
            exit(0);
        }

        std::cout << "FRAME RENDERED: " << frameMillis << std::endl;
        std::this_thread::sleep_for(
            (std::chrono::duration<double, std::milli>)frameMillis);
    }
    return 0;
}
