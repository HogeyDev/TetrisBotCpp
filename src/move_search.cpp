#include "move_search.hpp"
#include "constants.hpp"
#include "types.hpp"
#include "util.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

std::vector<Move> getPossibleMoves(Game* currentGame,
                                   std::vector<Game*>& searchedStates) {
    std::vector<Move> moveList = {};

    char pieceName = currentGame->activePiece->getName();
    int rotationStart = pieceName == 'O' ? 0 : -1;
    int rotationEnd = pieceName == 'O' || pieceName == 'I' ||
                              pieceName == 'S' || pieceName == 'Z'
                          ? 0
                          : 2;

    int xStart = pieceName == 'O' ? -4 : -5;
    int xEnd = 4;

    for (int xOffset = xStart; xOffset <= xEnd; xOffset++) {
        for (int rotationState = rotationStart; rotationState <= rotationEnd;
             rotationState++) {
            moveList.push_back({xOffset, rotationState});
            std::string inputTimeline =
                generateInputTimeline(tapTimeline, {xOffset, rotationState});
            Game* gameCopy = currentGame->clone();
            for (unsigned int i = 0; i < inputTimeline.size(); i++) {
                gameCopy->tick(inputTimeline.at(i));
            }
            searchedStates.push_back(gameCopy);
        }
    }
    return moveList;
}

std::string getBestMove(Game* currentGame) {
    return getBestMove(currentGame, 2);
}
std::string getBestMove(Game* currentGame, int searchDepth) {
    std::vector<Game*> searchedStates;
    std::vector<Move> generatedMoveList =
        getPossibleMoves(currentGame, searchedStates);
    std::vector<std::string> spinsAndTucks =
        searchForTucksAndSpins(currentGame, generatedMoveList, searchedStates);
    std::vector<std::string> unsortedTimelineList = {};
    std::vector<std::string> timelineList = {};
    std::vector<int> evaluationList = {};

    for (Move move : generatedMoveList) {
        unsortedTimelineList.push_back(
            generateInputTimeline(tapTimeline, move));
    }
    unsortedTimelineList.insert(unsortedTimelineList.end(),
                                spinsAndTucks.begin(), spinsAndTucks.end());

    // std::cout << "Searching " << unsortedTimelineList.size() << " Timelines"
    //           << std::endl;

    for (std::string timeline : unsortedTimelineList) {
        // std::cout << "Searching Timeline: " << timeline << std::endl;
        Game* gameCopy = currentGame->clone();

        gameCopy->simulatePiece(timeline);

        if (searchDepth > 1) {
            std::string bestSecondaryMove =
                getBestMove(gameCopy, searchDepth - 1);
            gameCopy->simulatePiece(bestSecondaryMove);
        }

        int currentEvaluation = evaluateGame(gameCopy);
        int insertionIndex =
            findEvaluationListIndex(evaluationList, currentEvaluation);

        // exit(0);

        timelineList.insert(timelineList.begin() + insertionIndex, timeline);
        evaluationList.insert(evaluationList.begin() + insertionIndex,
                              currentEvaluation);
    }

    return timelineList[0];
}

int findEvaluationListIndex(std::vector<int> evaluations,
                            int currentEvaluation) {
    if (evaluations.size() == 0)
        return 0;
    for (unsigned int i = 0; i < evaluations.size(); i++) {
        if (currentEvaluation < evaluations.at(i))
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
    int boardHeight = 0;
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
                columnHeight = (20 - y);
                break;
            }
        }
        boardHeight +=
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
    boardHeight = boardHeight * 100;

    total += boardHeight;
    total += bumpiness;
    total += totalHoleCount + minosAboveHole;
    total += linesCleared;
    total += wells;
    total += isOver;
    // total += isTetrisReady(game);
    // return boardHeight;
    return total;
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

std::vector<std::string>
searchForTucksAndSpins(Game* game, std::vector<Move> baseMoveList,
                       std::vector<Game*> searchedStates) {
    std::vector<std::string> tucks;
    std::vector<std::string> spins;
    std::vector<std::string> spinTucks;
    std::vector<Game*> allSeenGameStates;
    for (unsigned int i = 0; i < baseMoveList.size(); i++) {
        Game* gameCopy = game->clone();
        std::string inputTimeline =
            generateInputTimeline(tapTimeline, baseMoveList[i]);

        for (unsigned int i = 0; i < inputTimeline.size(); i++) {
            gameCopy->tick(inputTimeline[i]);
        }

        std::vector<std::string> tuckInputs = {"", "LR", "LR", "", "LR"};
        std::vector<std::string> spinInputs = {"", "", "A", "", "AB"};
        std::vector<std::string> spinTucksInputs = {"", "", "EI", "", "EFIG"};

        int numOrientations = gameCopy->activePiece->getRotationStates();

        int oldPieceTotal = gameCopy->totalPieces;
        int oldY = gameCopy->activePiece->y;

        int maxFrames = 20 * getFramesUntilPieceDrop(gameCopy->level);
        while (oldPieceTotal == gameCopy->totalPieces) {
            if (gameCopy->isOver)
                break;
            // only check for spins if inputting and piece has
            // dropped
            if (oldY != gameCopy->activePiece->y) {
                // tapTimeline[gameCopy->activePiece->frames] == 'X') {
                oldY = gameCopy->activePiece->y;
                // std::cout << "Searched" << std::endl;

                std::string possibleInputs = tuckInputs[numOrientations];

                for (unsigned int input = 0; input < possibleInputs.size();
                     input++) {
                    Game* gameCopyCopy = gameCopy->clone();
                    if (gameCopyCopy->tick(possibleInputs[input])) {
                        if (isNewState(allSeenGameStates, gameCopyCopy)) {
                            tucks.push_back(inputTimeline +
                                            possibleInputs[input]);
                            allSeenGameStates.push_back(gameCopyCopy);
                        }
                    }
                }
            }
            gameCopy->tick('.');
            inputTimeline += '.';
            if (gameCopy->activePiece->frames > maxFrames)
                break;
        }
    }

    // std::vector<std::string> complete;
    // complete.insert(complete.end(), tucks.begin(), tucks.end());
    // complete.insert(complete.end(), spins.begin(), spins.end());
    // complete.insert(complete.end(), spinTucks.begin(), spinTucks.end());
    return tucks;
}

bool isNewState(std::vector<Game*> searchedStates, Game* currentState) {
    if (searchedStates.size() == 0)
        return true;
    for (unsigned int i = 0; i < searchedStates.size(); i++) {
        Game* state = searchedStates.at(i);
        if (state->activePiece->y != currentState->activePiece->y)
            return true;
        if (state->activePiece->x != currentState->activePiece->y)
            return true;
    }
    return false;
}
