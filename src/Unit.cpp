/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#include "Unit.hpp"

UnitDefinition::UnitDefinition(Unit _unit,
                               unsigned int _hitsOn,
                               std::set<Unit>&& _counters,
                               Resources _cost,
                               unsigned int _rolls)
    : unit(_unit)
    , hitsOn(_hitsOn)
    , counters(std::move(_counters))
    , cost(_cost)
    , rolls(_rolls)
{
}

Unit
UnitDefinition::getType() const
{
    return unit;
}

unsigned int
UnitDefinition::doesHitOn() const
{
    return hitsOn;
}

bool
UnitDefinition::hasUnitsItCounters() const
{
    return not counters.empty();
}

bool
UnitDefinition::isCounterOf(Unit _unit) const
{
    return counters.find(_unit) != counters.end();
}

unsigned int
UnitDefinition::getRolls() const
{
    return rolls;
}

const Resources&
UnitDefinition::getCost() const
{
    return cost;
}
