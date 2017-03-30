/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

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
                   unsigned int _rolls = 1)
        : unit(_unit)
        , hitsOn(_hitsOn)
        , counters(std::move(_counters))
        , rolls(_rolls)
    {
    }

    UnitDefinition() = delete;
    ~UnitDefinition() = default;
    UnitDefinition(const UnitDefinition&) = default;
    UnitDefinition& operator=(const UnitDefinition&) = delete;
    UnitDefinition(UnitDefinition&&) = delete;
    UnitDefinition& operator=(UnitDefinition&&) = delete;

    Unit getType() const
    {
        return unit;
    }

    unsigned int doesHitOn() const
    {
        return hitsOn;
    }

    bool hasUnitsItCounters() const
    {
        return not counters.empty();
    }

    bool isCounterOf(Unit _unit) const
    {
        return counters.find(_unit) != counters.end();
    }

    unsigned int getRolls() const
    {
        return rolls;
    }

  private:
    Unit           unit;
    unsigned int   hitsOn;
    std::set<Unit> counters;
    unsigned int   rolls;
};
