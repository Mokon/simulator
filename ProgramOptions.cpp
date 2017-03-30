/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#include "ProgramOptions.hpp"

ProgramOptions::ProgramOptions()
    : boost::program_options::options_description("valid options")
{
    add_options()
        ("num-simulations", boost::program_options::value<unsigned int>(
            &numSimulations)->default_value(numSimulations),
            "sets the number of simulations to run")
        ("general-roll", boost::program_options::value<bool>(
            &generalRoll)->default_value(generalRoll),
            "turns on the general roll")
        ("risk-europe", boost::program_options::value<bool>(
            &riskEurope)->default_value(riskEurope),
            "sets up the simulator to run for risk europe")
        ("risk", boost::program_options::value<bool>(
            &risk)->default_value(risk),
            "sets up the simulator to run for risk")
        ("verbose", boost::program_options::value<bool>(
            &verbose)->default_value(verbose),
            "turns on verbose loggging")
        ;

    for (int a{ArmyTypeMin}; a <= ArmyTypeMax; ++a) {
        ArmyType army = static_cast<ArmyType>(a);
        for (int u{UnitMin}; u <= UnitMax; ++u) {
            Unit unit = static_cast<Unit>(u);

            std::string armyName{asString(army)};
            std::string unitName{asString(unit)};
            std::string longOption{armyName + "-" + unitName};
            add_options()
                (longOption.c_str(), boost::program_options::value<unsigned int>(
                    &armyUnitCounts[army][unit])->default_value(0),
                    "sets the specified unit count");
        }
    }
}

bool
ProgramOptions::parseArgs(int argc, char** argv)
{
    try {
        boost::program_options::variables_map vm;

        assert(argv != nullptr);
        assert(argv[0] != nullptr);

        boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, *this), vm);
        boost::program_options::notify(vm);
    } catch (...) {
        std::cerr << "Error parsing command line options, "
            << *this << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

Army
ProgramOptions::generateArmy(ArmyType army)
{
    LOG("Generating army " << asString(army) << std::endl);
    std::list<Unit> units;
    for (int u{UnitMin}; u <= UnitMax; ++u) {
        Unit unit = static_cast<Unit>(u);
        for (unsigned int c{0}; c < armyUnitCounts[army][unit]; ++c) {
            LOG("Adding unit " << asString(unit) << std::endl);
            units.push_back(unit);
        }
    }
    
    return Army(army, {units});
}
