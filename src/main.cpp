#include "move_search.hpp"
#include "rng.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#define FRAMERATE 60

static const float frameMillis = (float)1000 / FRAMERATE;

int main() {
    Game* game = new Game();

    std::string inputTimeline = "";
    unsigned int i = 0;

    while (true) {
        if (i >= inputTimeline.size()) {
            inputTimeline = generateInputTimeline("X.", getBestMove(game));
            i = 0;
        }

        game->tick(inputTimeline[i]);
        i++;

        system("clear");
        game->printBoard();
        std::cout << game->isOver << std::endl;
        std::cout << inputTimeline << std::endl;

        if (game->isOver) {
            exit(0);
        }

        std::this_thread::sleep_for(
            (std::chrono::duration<double, std::milli>)frameMillis);
    }
    return 0;
}