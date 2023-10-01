#pragma once

#include "game.hpp"
#include "types.hpp"

Move getBestMove(Game* currentGame, int searchDepth);
Move getBestMove(Game* currentGame);
std::string generateInputTimeline(std::string frameTimeline, Move move);