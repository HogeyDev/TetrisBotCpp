#include "move_search.hpp"
#include "types.hpp"
#include "util.hpp"
#include <algorithm>
#include <cstdlib>
#include <string>

Move getBestMove(Game* currentGame) { return getBestMove(currentGame, 2); }
Move getBestMove(Game* currentGame, int searchDepth) {
    std::vector<Move> moveList = {};
    for (int xOffset = -5; xOffset <= 4; xOffset++) {
        for (int rotationState = -1; rotationState <= 2; rotationState++) {
            Game* gameCopy = currentGame->clone();
            Move move = {xOffset, rotationState};

            gameCopy->simulatePiece(move);

            // TODO Eval And Sorting

            moveList.push_back(move);
        }
    }
    moveList = sortMoveListByEvaluation(moveList);
    return moveList[0];
}

std::vector<Move> sortMoveListByEvaluation(std::vector<Move> moveList) {
    return moveList;
}

int evaluateGame(Game* game) { return 0; }

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