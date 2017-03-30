/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

#include "Army.hpp"
#include "Dice.hpp"
#include "Unit.hpp"

#include <list>
#include <map>

enum Result
{
    AttackerWins,
    DefenderWins,
    Draw
};

class Simulator final
{
  public:
    Simulator(std::vector<UnitDefinition>&& _unitDefinitions,
              std::list<Unit>&& _rankedAttackOrders,
              bool _generalRoll);

    Simulator() = delete;
    ~Simulator() = default;
    Simulator(const Simulator&) = delete;
    Simulator& operator=(const Simulator&) = delete;
    Simulator(Simulator&&) = delete;
    Simulator& operator=(Simulator&&) = delete;

    Result simulate(Army attacker, Army defender) const;

  private:
    void doGeneralRoll(Army& attacker, Army& defender) const;

  private:
    Dice                           dice;
    std::map<Unit, UnitDefinition> unitDefinitions;
    std::list<Unit>                rankedAttackOrders;
    bool                           generalRoll;
};
