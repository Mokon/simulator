/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

#include "Army.hpp"
#include "Simulator.hpp"

#include <memory>

enum SimulatorType
{
    Risk,
    RiskEurope,
    Custom
};

class Simulators final
{
  public:
    Simulators();

    void makeCustomSimulator(bool generalRoll);

    ~Simulators() = default;
    Simulators(const Simulators&) = delete;
    Simulators& operator=(const Simulators&) = delete;
    Simulators(Simulators&&) = delete;
    Simulators& operator=(Simulators&&) = delete;

    void runSimulations(Army& attacker,
                        Army& defender,
                        SimulatorType& type,
                        unsigned int numSimulations);

    void runSimulations(Army& attacker,
                        Army& defender,
                        Simulator& simulation,
                        unsigned int numSimulations);

  private:
    Simulator                  risk;
    Simulator                  riskEurope;
    std::unique_ptr<Simulator> custom;
};
