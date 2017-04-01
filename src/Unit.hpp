/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

#include "Resources.hpp"

#include <cassert>
#include <cstdint>
#include <set>
#include <string>

enum Unit
    : uint8_t
{
    Peasent,
    Archer,
    Infantry,
    Cavalry,
    Pikemen,
    Siege,
    UnitMin = Peasent,
    UnitMax = Siege
};

inline std::string asString(Unit unit)
{
    switch (unit) {
        case Peasent:
            return "peasent";
        case Archer:
            return "archer";
        case Infantry:
            return "infantry";
        case Cavalry:
            return "cavalry";
        case Pikemen:
            return "pikemen";
        case Siege:
            return "siege";
        default:
            assert(false);
    }
}

class UnitDefinition final
{
  public:
    UnitDefinition(Unit _unit,
                   unsigned int _hitsOn,
                   std::set<Unit>&& _counters = {},
                   Resources _cost = {{{0u, 0u, 0u, 0u, 0u}}},
                   unsigned int _rolls = 1);

    UnitDefinition() = delete;
    ~UnitDefinition() = default;
    UnitDefinition(const UnitDefinition&) = default;
    UnitDefinition& operator=(const UnitDefinition&) = delete;
    UnitDefinition(UnitDefinition&&) = delete;
    UnitDefinition& operator=(UnitDefinition&&) = delete;

    Unit getType() const;

    unsigned int doesHitOn() const;

    bool hasUnitsItCounters() const;

    bool isCounterOf(Unit _unit) const;

    unsigned int getRolls() const;

    const Resources& getCost() const;

  private:
    Unit           unit;
    unsigned int   hitsOn;
    std::set<Unit> counters;
    Resources      cost;
    unsigned int   rolls;
};
