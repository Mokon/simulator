/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#include "ProgramOptions.hpp"
#include "Simulators.hpp"

int main(int argc, char** argv)
{
    ProgramOptions opts;
    if (opts.parseArgs(argc, argv) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    auto attacker = opts.generateArmy(Attacker);
    auto defender = opts.generateArmy(Defender);

    Simulators simulators;
    simulators.makeCustomSimulator(opts.generalRoll);

    SimulatorType type{Custom};
    if (opts.risk) {
        type = Risk;
    } else if (opts.riskEurope) {
        type = RiskEurope;
    }

    simulators.runSimulations(attacker,
                              defender,
                              type,
                              opts.numSimulations,
                              opts.showStats);
    return EXIT_SUCCESS;
}
