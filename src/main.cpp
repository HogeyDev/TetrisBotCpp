#include "constants.hpp"
#include "move_search.hpp"
#include "rng.hpp"
#include "util.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#define FRAMERATE 60

static const float frameMillis = (float)1000 / FRAMERATE;

int main() {
    Game* game = new Game(startingLevel);
    int oldPieceTotal = -1;

    std::string inputTimeline = "";
    unsigned int i = -1;

    // std::vector<Move> moveList = getPossibleMoves(game);
    // std::vector<std::string> tucks = searchForTucksAndSpins(game, moveList);
    // for (std::string tuck : tucks) {
    //   std::cout << tuck << std::endl;
    // }
    // exit(0);

    while (true) {
        if (oldPieceTotal != game->totalPieces) {
            inputTimeline = getBestMove(game, globalSearchDepth);
            i = 0;
            oldPieceTotal = game->totalPieces;
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
        std::cout << game->isOver << std::endl;
        std::cout << inputTimeline << std::endl;
        std::cout << evaluateGame(game) << std::endl;
        std::cout << getScareHeight(game->level) << std::endl;

        if (game->isOver) {
            std::cout << "GAME OVER!" << std::endl;
            exit(1);
        }

        std::cout << "FRAME RENDERED: " << frameMillis << std::endl;
        std::this_thread::sleep_for(
            (std::chrono::duration<double, std::milli>)frameMillis);
    }
    return 0;
}
