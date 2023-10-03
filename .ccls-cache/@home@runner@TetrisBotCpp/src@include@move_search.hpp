#pragma once

#include "game.hpp"
#include "types.hpp"

std::vector<Move> getPossibleMoves(Game *currentGame, std::vector<Game*>& searchedStates);
std::string getBestMove(Game *currentGame, int searchDepth);
std::string getBestMove(Game *currentGame);
int findEvaluationListIndex(std::vector<int> evaluations, int currentEval);
// std::vector<Move> sortMoveListByEvaluation(std::vector<Move> moveList);
int evaluateGame(Game *game);
std::string generateInputTimeline(std::string frameTimeline, Move move);
std::vector<std::string> searchForTucksAndSpins(Game *game,
                                                std::vector<Move> baseMoveList, std::vector<Game*> searchedStates);
bool isNewState(std::vector<Game*> searchedStates, Game* currentState);