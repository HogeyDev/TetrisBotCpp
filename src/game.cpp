#include "game.hpp"
#include "move_search.hpp"
#include "rng.hpp"
#include "types.hpp"
#include "util.hpp"
#include <string>

Game::Game(int startingLevel, int seed) {
    this->seed = seed;
    this->internalRng = new RNG::LFSR32();
    this->level = startingLevel;
    this->lines = 0;
    this->score = 0;
    this->board = new Board();
    this->activePiece = this->generatePiece();
    this->previewPiece = this->generatePiece();
    this->frames = 0;
    this->isOver = false;
    this->linesLastCleared = 0;
}

void Game::printBoard() {
    PieceMatrix pieceMatrix = this->activePiece->getMatrix();

    std::string output = "+--------------------+\n";
    for (int y = 0; y < 20; y++) {
        std::string row = "|";
        for (int x = 0; x < 10; x++) {
            std::string character =
                (this->board->getMinoXY(x, y) > 0 ? "[]" : "  ");

            int iy = y - this->activePiece->y;
            int ix = x - this->activePiece->x;
            if (iy >= 0 && iy < (int)pieceMatrix.size()) {
                if (ix >= 0 && ix < (int)pieceMatrix[iy].size()) {
                    if (pieceMatrix.at(iy).at(ix)) {
                        character = "[]";
                    }
                }
            }

            row += character;
        }
        row += "|\n";
        output += row;
    }
    output += "+--------------------+";
    std::cout << output << std::endl;
}

Game* Game::clone() {
    Game* copy = new Game();
    copy->level = this->level;
    copy->score = this->score;
    copy->lines = this->lines;
    copy->frames = this->frames;
    copy->totalPieces = this->totalPieces;
    copy->lastFrameTime = this->lastFrameTime;
    copy->seed = this->seed;
    copy->linesLastCleared = this->linesLastCleared;
    copy->isOver = this->isOver;
    copy->board = this->board->clone();
    copy->activePiece = this->activePiece->clone();
    copy->previewPiece = this->previewPiece->clone();
    copy->internalRng = this->internalRng->clone();
    return copy;
}

Piece* Game::getNewPiece() {
    this->totalPieces++;
    this->activePiece = this->previewPiece->clone();
    return (this->previewPiece =
                new Piece(this->internalRng->getRangedInt(0, 6)));
}

Piece* Game::generatePiece() {
    this->totalPieces++;
    return new Piece(this->internalRng->getRangedInt(0, 6));
}

bool Game::isGravityFrame() {
    int gravityFrames = getFramesUntilPieceDrop(this->level);
    return (this->activePiece->frames % gravityFrames == gravityFrames - 1);
}

void Game::simulatePiece(Move move) {
    std::string inputTimeline = generateInputTimeline("X.", move);
    for (unsigned int i = 0; i < inputTimeline.size(); i++) {
        this->tick(inputTimeline.at(i));
    }
    int oldPieceCount = this->totalPieces;
    while (oldPieceCount == this->totalPieces) {
        this->tick('.');
    }
}

void Game::tick(char movementCharacter) {
    if (this->isOver)
        return;
    if (this->toppedOut()) {
        this->isOver = true;
        return;
    }
    if (movementCharacter != '.')
        this->handleMovementCharacter(movementCharacter);
    if (this->isGravityFrame())
        this->tryPieceDrop();
    this->activePiece->frames++;
    this->frames++;

    this->removeFilledLines();
}

void Game::tryPieceDrop() {
    this->activePiece->y++;
    if (this->pieceCollidingWithBoard()) {
        this->activePiece->y--;
        this->board->addPieceToBoard(this->activePiece);
        this->getNewPiece();
    }
}

void Game::removeFilledLines() {
    std::vector<int> rowsFilled;
    for (int y = 0; y < 20; y++) {
        bool rowFilled = true;
        for (int x = 0; x < 10; x++) {
            if (!this->board->getMinoXY(x, y)) {
                rowFilled = false;
                break;
            }
        }
        if (rowFilled) {
            rowsFilled.push_back(y);
        }
    }
    this->lines += rowsFilled.size();
    for (unsigned int i = 0; i < rowsFilled.size(); i++) {
        this->board->removeRow(rowsFilled[i]);
    }
    if (rowsFilled.size() > 0)
        this->linesLastCleared = rowsFilled.size();
}

void Game::handleMovementCharacter(char movementCharacter) {
    switch (movementCharacter) {
    case 'L':
        this->tryXMovement(-1);
        break;
    case 'R':
        this->tryXMovement(1);
        break;
    case 'A':
        this->tryRotation(1);
        break;
    case 'B':
        this->tryRotation(-1);
        break;
    case 'E':
        this->tryXMovement(-1);
        this->tryRotation(1);
        break;
    case 'F':
        this->tryXMovement(-1);
        this->tryRotation(-1);
        break;
    case 'I':
        this->tryXMovement(1);
        this->tryRotation(1);
        break;
    case 'G':
        this->tryXMovement(1);
        this->tryRotation(-1);
        break;
    case '.':
        break;
    }
}

bool Game::tryXMovement(int xDirection) {
    this->activePiece->x += xDirection;
    if (this->pieceCollidingWithBoard()) {
        this->activePiece->x -= xDirection;
        return false;
    }
    return true;
}

bool Game::tryRotation(int rotationDirection) {
    this->activePiece->rotate(rotationDirection);
    if (this->pieceCollidingWithBoard()) {
        this->activePiece->rotate(-rotationDirection);
        return false;
    }
    return true;
}

bool Game::pieceCanDrop() {
    this->activePiece->y++;
    bool colliding = this->pieceCollidingWithBoard();
    this->activePiece->y--;
    return colliding;
}

bool Game::pieceCollidingWithBoard() {
    PieceMatrix pieceMatrix = this->activePiece->getMatrix();
    for (unsigned int iy = 0; iy < pieceMatrix.size(); iy++) {
        for (unsigned int ix = 0; ix < pieceMatrix[iy].size(); ix++) {
            int x = this->activePiece->x + ix;
            int y = this->activePiece->y + iy;
            if (pieceMatrix[iy][ix] > 0) {
                if (y > 19 || (x < 0 || x > 9))
                    return true;
                if (this->board->getMinoXY(x, y))
                    return true;
            }
        }
    }
    return false;
}

bool Game::toppedOut() {
    if (this->pieceCollidingWithBoard())
        return true;
    return false;
}
