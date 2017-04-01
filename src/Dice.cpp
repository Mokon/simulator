/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#include "Dice.hpp"

Dice::Dice(unsigned int sides,
           unsigned int start)
    : randomDevice()
    , generator(randomDevice())
    , distribution(start, start + sides - 1)
{
}

unsigned int
Dice::roll() const
{
    return distribution(generator);
}

std::priority_queue<unsigned int>
Dice::roll(unsigned long times) const
{
    std::priority_queue<unsigned int> rolls;

    for (unsigned int i{0}; i < times; ++i) {
        rolls.emplace(roll());
    }

    return rolls;
}
