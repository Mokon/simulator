/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

#include "Army.hpp"
#include "Unit.hpp"

#include <array>

enum Winner
{
    AttackerWins,
    DefenderWins,
    Draw
};

class Result final
{
  public:
    Result(unsigned int _numRounds, Army* winner = nullptr);

    ~Result() = default;
    Result(const Result&) = delete;
    Result& operator=(const Result&) = delete;
    Result(Result&&) = default;
    Result& operator=(Result&&) = delete;

    static Winner determineWinner(Army* _winner);

  public:
    const unsigned int                    numRounds;
    const Winner                          winner;
    std::array<unsigned int, UnitMax + 1> unitsRemaining{};
};
