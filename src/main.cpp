#include "move_search.hpp"
#include "rng.hpp"
#include <iostream>

int main() {
    RNG::LFSR32* rng = new RNG::LFSR32();
    for (int i = 0; i < 32; i++) {
        std::cout << rng->getRangedInt(0, 10) << std::endl;
    }
    return 0;
}
