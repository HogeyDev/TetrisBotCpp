#pragma once

#include "game.hpp"
#include "types.hpp"

Move getBestMove(Game* currentGame, int searchDepth);
Move getBestMove(Game* currentGame);
std::vector<Move> sortMoveListByEvaluation(std::vector<Move> moveList);
int evaluateGame(Game* game);
std::string generateInputTimeline(std::string frameTimeline, Move move);