/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

#include "Army.hpp"
#include "Unit.hpp"

#include <array>
#include <boost/program_options.hpp>

class ProgramOptions final
    : public boost::program_options::options_description
{
  public:
    ProgramOptions();

    ~ProgramOptions() = default;
    ProgramOptions(const ProgramOptions&) = delete;
    ProgramOptions& operator=(const ProgramOptions&) = delete;
    ProgramOptions(ProgramOptions&&) = delete;
    ProgramOptions& operator=(ProgramOptions&&) = delete;

    bool parseArgs(int argc, char** argv);

    Army generateArmy(ArmyType army);

  public:
    unsigned int numSimulations{1};
    bool         showStats{false};
    bool         generalRoll{false};
    bool         riskEurope{false};
    bool         risk{false};

  private:
    std::array<std::array<unsigned int, UnitMax + 1>,
               ArmyTypeMax + 1> armyUnitCounts{};
};
