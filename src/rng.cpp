#include "rng.hpp"
#include <iostream>

RNG::LFSR32::LFSR32(unsigned int state) {
    std::cout << state << std::endl;
    this->registers = state;
    this->taps = {32, 22, 2, 1};
}

unsigned int RNG::LFSR32::nextState() {
    int bit = 0;
    for (auto& tap : this->taps) {
        int shiftNumber = 32 - tap;
        bit ^= (this->registers >> shiftNumber);
    }
    this->registers = (this->registers >> 1) | (bit << 31);
    return this->registers;
}

float RNG::LFSR32::getFloat() {
    unsigned int integer = this->nextState();
    return ((float)integer / (1u << 31)) / 2;
}

RNG::LFSR32* RNG::LFSR32::clone() {
    RNG::LFSR32* copy = new RNG::LFSR32();
    copy->registers = this->registers;
    for (unsigned int i = 0; i < this->taps.size(); i++) {
        copy->taps[i] = this->taps[i];
    }
    return copy;
}

int RNG::LFSR32::getRangedInt(int min, int max) {
    int range = max - min + 1;
    return (min + (this->nextState() % range));
}