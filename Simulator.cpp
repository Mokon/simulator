/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#include "Simulator.hpp"

#include "Log.hpp"

#include <algorithm>

Simulator::Simulator(std::vector<UnitDefinition>&& _unitDefinitions,
                     std::list<Unit>&& _rankedAttackOrders,
                     bool _generalRoll)
    : unitDefinitions()
    , rankedAttackOrders(std::move(_rankedAttackOrders))
    , generalRoll(_generalRoll)
{
    for (auto& def : _unitDefinitions) {
        unitDefinitions.emplace(def.getType(), def);
    }
}

Result
Simulator::simulate(Army attacker, Army defender) const
{
    LOG(std::endl << "Simulation:" << std::endl);

    unsigned int roundNumber{1};
    while (not attacker.destroyed() and not defender.destroyed()) {
        LOG("\tRound " << roundNumber << std::endl);

        for (auto& rankedAttackOrder : rankedAttackOrders) {
            LOG("\t\tranked attack order "
                << asString(rankedAttackOrder) << std::endl);

            const UnitDefinition& unitDef = unitDefinitions.at(rankedAttackOrder);
            auto offenseHits = attacker.rollDiceAndCountHits(unitDef, dice);
            auto defenseHits = defender.rollDiceAndCountHits(unitDef, dice);

            attacker.takeHits(unitDef, defenseHits);
            defender.takeHits(unitDef, offenseHits);

            if (attacker.destroyed() or defender.destroyed()) {
                break;
            }
        }

        if (generalRoll) {
            doGeneralRoll(attacker, defender);
        }

        ++roundNumber;
    }

    if (attacker.destroyed() and defender.destroyed()) {
        LOG("\tDraw, both armies destroyed" << std::endl);
        return Draw;
    } else if (defender.destroyed()) {
        LOG("\tAttacker Wins" << std::endl);
        return AttackerWins;
    } else if (attacker.destroyed()) {
        LOG("\tDefense Wins" << std::endl);
        return DefenderWins;
    } else {
        assert(false);
    }
}

void
Simulator::doGeneralRoll(Army& attacker, Army& defender) const
{
    auto attackerDice = std::min(3ul, attacker.size());
    auto defenderDice = std::min(2ul, defender.size());

    LOG("\t\tgeneral roll " << attackerDice << "a vs. "
        << defenderDice <<"d" << std::endl);

    auto attackerRolls = dice.roll(attackerDice);
    auto defenderRolls = dice.roll(defenderDice);

    while (not attackerRolls.empty() and not defenderRolls.empty()) {
        LOG("\t\t\tdice rolls were " << attackerRolls.top() << "a vs. "
            << defenderRolls.top() << "d" << std::endl);

        if (attackerRolls.top() > defenderRolls.top()) {
            defender.takeHits(1);
        } else {
            attacker.takeHits(1);
        }

        attackerRolls.pop();
        defenderRolls.pop();
    }
}
