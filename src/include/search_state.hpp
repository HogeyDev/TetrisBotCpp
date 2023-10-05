#pragma once

#include "game.hpp"
#include <string>

class SearchState {
  public:
    Game *game;
    std::string inputTimeline;
    SearchState();
    SearchState(Game *game);
    SearchState(std::string inputTimeline);
    SearchState(Game *game, std::string inputTimeline);
};
