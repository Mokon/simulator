/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#include "Result.hpp"

Result::Result(unsigned int _numRounds, Army* _winner)
    : numRounds(_numRounds)
    , winner(determineWinner(_winner))
    , unitsRemaining()
{
    if (_winner != nullptr) {
        unitsRemaining = _winner->summary();
    }
}

Winner
Result::determineWinner(Army* _winner)
{
    if (_winner == nullptr) {
        return Draw;
    } else {

        switch (_winner->getType()) {
            case Attacker:
                return AttackerWins;
            case Defender:
                return DefenderWins;
            default:
                assert(false);
        }
    }
}
