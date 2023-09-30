#include "board.hpp"
#include "rng.hpp"

class Game {
  public:
    int level;
    int score;
    int lines;
    int frames;
    int startingTime;
    int totalPieces;
    int lastFrameTime;
    int seed;
    int linesLastCleared;
    bool isOver;
    Board* board;
    Piece* activePiece;
    Piece* previewPiece;
    RNG::LFSR32* internalRng;

    Game(int startingLevel = 18);

    Game* clone() {
        Game* copy = new Game();
        copy->level = this->level;
        copy->score = this->score;
        copy->lines = this->lines;
        copy->frames = this->frames;
        copy->startingTime = this->startingTime;
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
    Piece* getNewPiece() {
        this->totalPieces++;
        this->activePiece = this->previewPiece->clone();
        return (this->previewPiece =
                    new Piece(this->internalRng->getRangedInt(0, 6)));
    }
    void tick();
};
