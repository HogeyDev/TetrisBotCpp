#pragma once

#include <string>

int getFramesUntilPieceDrop(int level);
int sign(int number);
int getScareHeight(int level);
int getMaximumNTap(int numberOfTaps, int level);
bool isInputFrame(std::string inputTimeline, int frame);

static const int lineClearValues[] = {0, 40, 100, 300, 1200};