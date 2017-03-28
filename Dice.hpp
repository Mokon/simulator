#pragma once

#include <random>
#include <queue>

class Dice final
{
  public:
    Dice(unsigned int sides = 6,
         unsigned int start = 1)
        : randomDevice()
        , generator(randomDevice())
        , distribution(start, start + sides - 1)
    {
    }

    ~Dice() = default;
    Dice(const Dice&) = delete;
    Dice& operator=(const Dice&) = delete;
    Dice(Dice&&) = delete;
    Dice& operator=(Dice&&) = delete;

    unsigned int roll()
    {
        return distribution(generator);
    }

    std::priority_queue<unsigned int> roll(unsigned int times)
    {
        std::priority_queue<unsigned int> rolls;

        for (unsigned int i{0}; i < times; ++i) {
            rolls.emplace(roll());
        }

        return rolls;
    }

  private:
    std::random_device                 randomDevice;
    std::mt19937                       generator;
    std::uniform_int_distribution<int> distribution;
};
