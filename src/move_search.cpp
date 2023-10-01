#include "move_search.hpp"
#include "constants.hpp"
#include "types.hpp"
#include "util.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <string>

Move getBestMove(Game* currentGame) { return getBestMove(currentGame, 2); }
Move getBestMove(Game* currentGame, int searchDepth) {
    std::vector<Move> moveList = {};
    std::vector<int> evaluationList = {};
    for (int xOffset = -5; xOffset <= 4; xOffset++) {
        for (int rotationState = -1; rotationState <= 2; rotationState++) {
            Game* gameCopy = currentGame->clone();
            Move move = {xOffset, rotationState};

            gameCopy->simulatePiece(move);

            if (searchDepth > 1) {
                Move bestSecondaryMove = getBestMove(gameCopy, searchDepth - 1);
                gameCopy->simulatePiece(bestSecondaryMove);
            }

            int currentEvaluation = evaluateGame(gameCopy);
            int insertionIndex =
                findEvaluationListIndex(evaluationList, currentEvaluation);

            moveList.insert(moveList.begin() + insertionIndex, move);
            evaluationList.insert(evaluationList.begin() + insertionIndex,
                                  currentEvaluation);

            // moveList.push_back(move);
        }
    }
    // moveList = sortMoveListByEvaluation(moveList);
    // for (unsigned int i = 0; i < evaluationList.size(); i++) {
    //     std::cout << evaluationList[i] << std::endl;
    // }
    // std::cout << std::endl;
    // exit(0);
    return moveList[0];
}

int findEvaluationListIndex(std::vector<int> evaluations,
                            int currentEvaluation) {
    if (evaluations.size() == 0)
        return 0;
    for (unsigned int i = 0; i < evaluations.size(); i++) {
        if (currentEvaluation <= evaluations.at(i))
            return i;
    }
    return evaluations.size();
    // std::cerr << "How Did You Get Here?" << std::endl;
    // exit(1);
}

// std::vector<Move> insertMoveByEvaluation(std::vector<Move> moveList) {
//     std::vector<Move> returnList;
//     std::vector<Move> newList = moveList;

//     unsigned int i = 0;
//     while (newList.size() > 0) {
//         if (evaluateGame(newList[i])) {
//         }
//         returnList.push_back();
//         i++;
//     }

//     return moveList;
// }

int evaluateGame(Game* game) {
    // lower is better
    int total = 0;
    int isOver = 0;
    int columnHeight = 0;
    int minosAboveHole = 0;
    int totalHoleCount = 0;
    int bumpiness = 0;
    int wells = 0;
    int linesCleared = 0;

    if (game->isOver)
        return INT32_MAX;

    for (int x = 0; x < 10; x++) {
        int columnHeight = 0;
        for (int y = 0; y < 20; y++) {
            if (game->board->getMinoXY(x, y)) {
                columnHeight = (19 - y);
                break;
            }
        }
        columnHeight +=
            (columnHeight >= getScareHeight(game->level) ? columnHeight : 0);
    }
    // evaluation += game.totalPieces * 1000;
    for (int x = 0; x < 10; x++) {
        bool hasHole = false;
        for (int y = 19; y >= 0; y--) {
            if (hasHole && game->board->getMinoXY(x, y)) {
                minosAboveHole++;
            }
            if (game->board->getMinoXY(x, y) == 0) {
                if ((game->board->getMinoXY(x - 1, y) ||
                     game->board->getMinoXY(x + 1, y)) &&
                    game->board->getMinoXY(x, y - 1)) {
                    hasHole = true;
                    totalHoleCount++;
                }
            }
        }
    }
    std::vector<int> heights;
    std::vector<int> deltaHeights;
    for (int x = 0; x < 10; x++) {
        int height = 0;
        for (int y = 0; y < 20; y++) {
            if (game->board->getMinoXY(x, y)) {
                height = (19 - y);
                break;
            }
        }
        heights.push_back(height);
    }
    for (unsigned int i = 0; i < heights.size() - 1; i++) {
        deltaHeights.push_back(heights[i] - heights[i + 1]);
    }
    for (unsigned int i = 0; i < deltaHeights.size(); i++) {
        bumpiness += std::abs(std::pow(deltaHeights[i], 2));
        if (i <= deltaHeights.size() - 1) {
            if (deltaHeights[i] <= -3 && deltaHeights[i + 1] >= 3) {
                wells++;
            }
        }
    }

    linesCleared = (game->linesLastCleared == 4
                        ? -1200
                        : std::pow(game->linesLastCleared, 2) * 200);
    bumpiness = bumpiness * 3;
    totalHoleCount = totalHoleCount * 1000;
    minosAboveHole = minosAboveHole * 1000;
    wells = (wells > 0 ? wells : 0) * 500;
    columnHeight = columnHeight * 100;

    total += columnHeight;
    total += bumpiness;
    total += totalHoleCount + minosAboveHole;
    total += linesCleared;
    total += wells;
    total += isOver;
    // total += isTetrisReady(game);
    return total;

    return 0;
}

std::string generateInputTimeline(std::string frameTimeline, Move move) {
    // L = Left
    // R = Right
    // A = Rotation Clockwise
    // B = Rotation Counter-Clockwise
    // E = L + A
    // F = L + B
    // I = R + A
    // G = R + B
    std::string outputString;
    std::string realFrameTimeline = frameTimeline;

    int rotationDirection = sign(move[1]);
    int movementDirection = sign(move[0]);

    int rotationsRemaining = abs(move[1]);
    int movementsRemaining = abs(move[0]);

    unsigned int i = 0;
    while ((rotationsRemaining + movementsRemaining) > 0) {
        if (i == realFrameTimeline.size())
            realFrameTimeline += frameTimeline;
        char character = '.';
        if (realFrameTimeline[i] == 'X' &&
            (rotationsRemaining + movementsRemaining) > 0) {
            if (movementsRemaining && rotationsRemaining) {
                // E = L + A
                // F = L + B
                // I = R + A
                // G = R + B
                if (movementDirection == -1) {
                    if (rotationDirection == 1) {
                        character = 'E';
                    } else if (rotationDirection == -1) {
                        character = 'F';
                    }
                } else if (movementDirection == 1) {
                    if (rotationDirection == 1) {
                        character = 'I';
                    } else if (rotationDirection == -1) {
                        character = 'G';
                    }
                }
            } else if (movementsRemaining) {
                // L = Left
                // R = Right
                if (movementDirection == -1) {
                    character = 'L';
                } else if (movementDirection == 1) {
                    character = 'R';
                }
            } else if (rotationsRemaining) {
                // A = Rotation Clockwise
                // B = Rotation Counter-Clockwise
                if (rotationDirection == -1) {
                    character = 'B';
                } else if (rotationDirection == 1) {
                    character = 'A';
                }
            }
            if (movementsRemaining > 0)
                movementsRemaining--;
            if (rotationsRemaining > 0)
                rotationsRemaining--;
        }
        outputString += character;
        i++;
    }
    return outputString;
}