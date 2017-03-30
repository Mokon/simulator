/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#include "Simulators.hpp"

Simulators::Simulators()
    : risk({
            {Peasent, 7,},
            {Archer, 7},
            {Infantry, 7},
            {Cavalry, 7},
            {Siege, 7,}
        },
        {}, true)
    , riskEurope({
            {Peasent, 7,},
            {Archer, 5},
            {Infantry, 7},
            {Cavalry, 3},
            {Siege, 3, {}, 2}
        },
        {
            Siege,
            Archer,
            Cavalry
        }, true)
    , custom(nullptr)
{
}

void
Simulators::makeCustomSimulator(bool generalRoll)
{
    custom.reset(new Simulator({
            {Peasent, 6},
            {Archer, 4, {Infantry}},
            {Infantry, 3, {Cavalry}},
            {Cavalry, 2, {Archer}},
            {Siege, 2, {Archer}, 2}
        },
        {
            Siege,
            Archer,
            Cavalry,
            Infantry,
            Peasent
        }, generalRoll));
}

void
Simulators::runSimulations(Army& attacker,
                           Army& defender,
                           SimulatorType& type,
                           unsigned int numSimulations)
{
    switch(type) {
        case Risk:
            runSimulations(attacker, defender, risk, numSimulations);
            break;
        case RiskEurope:
            runSimulations(attacker, defender, riskEurope, numSimulations);
            break;
        case Custom:
            assert (custom != nullptr);
            runSimulations(attacker, defender, *custom, numSimulations);
            break;
    }
}

void
Simulators::runSimulations(Army& attacker,
                           Army& defender,
                           Simulator& simulation,
                           unsigned int numSimulations)
{
    unsigned int results[3] = {};
    for (unsigned int sim{0}; sim < numSimulations; ++sim) {
        ++results[simulation.simulate(attacker, defender)];
    }

    // TODO compute stats
    std::cout << "After " << numSimulations
        << " simulations:" << std::endl;
    std::cout << "\tthere were " << results[Draw] << " draws" << std::endl;
    std::cout << "\tthere were " << results[AttackerWins]
        << " attacker victories" << std::endl;
    std::cout << "\tthere were " << results[DefenderWins]
        << " defender victories" << std::endl;
}
