/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#include "Simulators.hpp"

#include "Results.hpp"

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
            {Peasent,  7, {}, {{{0,  0, 0, 0, 0}}}, 2},
            {Infantry, 7, {}, {{{1,  0, 0, 0, 0}}}, 2},
            {Archer,   5, {}, {{{2,  0, 0, 0, 0}}}, 2},
            {Cavalry,  3, {}, {{{3,  0, 0, 0, 0}}}, 2},
            {Siege,    3, {}, {{{10, 0, 0, 0, 0}}}, 2}
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
            {Siege, 2, {Archer}, {{{0, 0, 0, 0, 0}}}, 2}
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
                           unsigned int numSimulations,
                           bool showStats)
{
    switch(type) {
        case Risk:
            runSimulations(attacker, defender, risk, numSimulations, showStats);
            break;
        case RiskEurope:
            runSimulations(attacker, defender, riskEurope, numSimulations,
                           showStats);
            break;
        case Custom:
            assert (custom != nullptr);
            runSimulations(attacker, defender, *custom, numSimulations,
                           showStats);
            break;
    }
}

void
Simulators::runSimulations(Army& attacker,
                           Army& defender,
                           Simulator& simulator,
                           unsigned int numSimulations,
                           bool showStats)
{
    Results results;
    for (unsigned int sim{0}; sim < numSimulations; ++sim) {
        results.add(simulator.simulate(attacker, defender));
    }

    std::cout << results.summary(showStats,
                                 simulator,
                                 attacker.getValue(simulator),
                                 defender.getValue(simulator));
}
