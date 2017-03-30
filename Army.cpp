/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#include "Army.hpp"

#include "Dice.hpp"

#include <algorithm>

Army::Army(ArmyType _type,
           std::list<Unit>& _units)
    : type(_type)
    , units(_units)
{
}

unsigned int
Army::rollDiceAndCountHits(const UnitDefinition& unitRolling,
                           const Dice& dice) const
{
    unsigned int hits{0};
    unsigned int rolls = count(unitRolling.getType()) * unitRolling.getRolls();

    if (rolls > 0) {
        LOG("\t\t\t" << asString(type) << " rolled ");

        for (unsigned int roll{0} ; roll < rolls ; ++roll) {
            unsigned int value{dice.roll()};

            LOG(value << ( roll + 1 == rolls ? "": ","));

            if (value >= unitRolling.doesHitOn()) {
                ++hits;
            }
        }

        LOG(std::endl);
    }

    return hits;
}

unsigned int
Army::count(Unit unit) const
{
    return static_cast<unsigned int>(std::count(units.begin(),
                                                units.end(),
                                                unit));
}

unsigned long
Army::size() const
{
    return units.size();
}

void
Army::takeHits(const UnitDefinition& attackingUnit, unsigned int hits)
{
            LOG(__LINE__ <<std::endl;);
    if (attackingUnit.hasUnitsItCounters()) {
            LOG(__LINE__ <<std::endl;);
        takeHits(hits,
                 "counter " + asString(attackingUnit.getType()),
                 [&attackingUnit](Unit unit) {
                     return attackingUnit.isCounterOf(unit);
                 });
    }

            LOG(__LINE__ <<std::endl;);
    takeHits(hits,
             "non-counter " + asString(attackingUnit.getType()),
             [](Unit) { return true; });

            LOG(__LINE__ <<std::endl;);
    if (hits != 0) {
        LOG("\t\t\t" << asString(type) << " " << hits
            << " unused hits" << std::endl);
    }
}

void
Army::takeHits(unsigned int hits)
{
    takeHits(hits, "general roll", [](Unit) { return true; });

    if (hits != 0) {
        LOG("\t\t\t" << asString(type) << " " << hits
            << " unused hits" << std::endl);
    }
}

bool
Army::destroyed() const
{
    return units.empty();
}

std::array<unsigned int, UnitMax + 1>
Army::summary() const
{
    std::array<unsigned int, UnitMax + 1> unitCounts;
    for (Unit unit : units) {
        ++unitCounts[unit];
    }
    return unitCounts;
}

void
Army::takeHits(unsigned int& hits,
               std::string source,
               std::function<bool(Unit)> condition)
{
            LOG(__LINE__ <<std::endl;);
    for (auto unit = units.begin(); unit != units.end();) {
            LOG(__LINE__ <<std::endl;);
        if (hits == 0) {
            break;
        }

            LOG(__LINE__ <<std::endl;);
        if (condition(*unit)) {
            LOG("\t\t\t" << asString(type) << " looses "
                << asString(*unit) << " from " << source << std::endl);
            unit = units.erase(unit);
            --hits;
            LOG(__LINE__ <<std::endl;);
        } else {
            ++unit;
        }
    }
}