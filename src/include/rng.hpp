#pragma once

#include <vector>
namespace RNG {
class LFSR32 {
    unsigned int registers;
    std::vector<int> taps;

  public:
    LFSR32(unsigned int state = 2273142425);
    unsigned int nextState();
    int getRangedInt(int min, int max);
    float getFloat();
    RNG::LFSR32* clone();
};

// class NES {
//   public:
//     int state;
//     NES() {}
// };
} // namespace RNG