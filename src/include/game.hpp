#pragma once

#include "board.hpp"
#include "rng.hpp"

class Game {
public:
  int level;
  int score;
  int lines;
  int frames;
  int totalPieces;
  int lastFrameTime;
  int seed;
  int linesLastCleared;
  bool isOver;
  Board *board;
  Piece *activePiece;
  Piece *previewPiece;
  RNG::LFSR32 *internalRng;

  Game(int startingLevel = 18, int seed = -1);

  void printPiecePreview();
  void printBoard();
  Game *clone();
  Piece *getNewPiece();
  Piece *generatePiece();
  bool isGravityFrame();
  void simulatePiece(Move move);
  void simulatePiece(std::string inputTimeline);
  bool tick(char movementCharacter = '.');
  void tryPieceDrop();
  void removeFilledLines();
  bool handleMovementCharacter(char movementCharacter);
  bool tryXMovement(int xDirection);
  bool tryRotation(int rotationDirection);
  bool pieceCanDrop();
  bool pieceCollidingWithBoard();
  bool toppedOut();
};
