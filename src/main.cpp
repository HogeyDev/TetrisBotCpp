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
    Game* game = new Game(18);
    int oldPieceTotal = -1;

    std::string inputTimeline = "";
    unsigned int i = -1;

    while (true) {
        if (oldPieceTotal != game->totalPieces) {
            inputTimeline = getBestMove(game, 3);
            i = 0;
            oldPieceTotal = game->totalPieces;
        }

        char movementCharacter = '.';
        if (i < inputTimeline.size())
            movementCharacter = inputTimeline[i];
        game->tick(movementCharacter);
        i++;

        system("clear");
        game->printPiecePreview();
        game->printBoard();
        std::cout << game->isOver << std::endl;
        std::cout << inputTimeline << std::endl;
        std::cout << evaluateGame(game) << std::endl;

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
