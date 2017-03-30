/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

#include "Log.hpp"
#include "Unit.hpp"

#include <array>
#include <functional>
#include <list>

class Dice;

enum ArmyType
{
    Attacker,
    Defender,
    ArmyTypeMin = Attacker,
    ArmyTypeMax = Defender
};

inline std::string asString(ArmyType army)
{
    switch (army) {
        case Attacker:
            return "attacker";
        case Defender:
            return "defender";
        default:
            assert(false);
    }
}

class Army final
{
  public:
    Army(ArmyType _type, std::list<Unit>& _units);

    Army() = delete;
    ~Army() = default;
    Army(const Army&) = default;
    Army& operator=(const Army&) = delete;
    Army(Army&&) = default;
    Army& operator=(Army&&) = delete;

    unsigned int rollDiceAndCountHits(const UnitDefinition& unitRolling,
                                      const Dice& dice) const;

    long count(Unit unit) const;

    unsigned long size() const;
    
    bool destroyed() const;

    ArmyType getType() const;

    void takeHits(const UnitDefinition& attackingUnit, unsigned int hits);

    void takeHits(unsigned int hits);

    std::array<unsigned int, UnitMax + 1> summary() const;

  private:
    void takeHits(unsigned int& hits,
                  std::string source,
                  std::function<bool(Unit)> condition);

  private:
    ArmyType        type;
    std::list<Unit> units; // TODO prob. would be better to be a array of counts
};
