#include "search_state.hpp"

SearchState::SearchState() {}

SearchState::SearchState(Game *game) {
	this->game = game;
}

SearchState::SearchState(std::string inputTimeline) {
	this->inputTimeline = inputTimeline;
}

SearchState::SearchState(Game *game, std::string inputTimeline) {
	this->game = game;
	this->inputTimeline = inputTimeline;
}
