#include "util.hpp"
#include <iostream>

int getFramesUntilPieceDrop(int level) {
    if (level >= 16 && level <= 18)
        return 3;
    if (level >= 19 && level <= 28)
        return 2;
    if (level >= 29)
        return 1;
    std::cerr << "Unimplemented Level Gravity: " << level << std::endl;
    exit(1);
}

int sign(int number) {
    if (number < 0)
        return -1;
    if (number > 0)
        return 1;
    return 0;
}