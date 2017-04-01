/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

#include <random>
#include <queue>

class Dice final
{
  public:
    Dice(unsigned int sides = 6, unsigned int start = 1);

    ~Dice() = default;
    Dice(const Dice&) = delete;
    Dice& operator=(const Dice&) = delete;
    Dice(Dice&&) = delete;
    Dice& operator=(Dice&&) = delete;

    unsigned int roll() const;

    std::priority_queue<unsigned int> roll(unsigned long times) const;

  private:
    std::random_device                                  randomDevice;
    mutable std::mt19937                                generator;
    mutable std::uniform_int_distribution<unsigned int> distribution;
};
