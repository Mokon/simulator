#include "Dice.hpp"

#include <algorithm>
#include <boost/program_options.hpp>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <vector>

static bool verbose{false};

#define LOG(msg) \
    do { \
        if (verbose) { \
            std::cout << msg; \
        } \
    } while(false)

enum Unit
{
    Peasent,
    Archer,
    Infantry,
    Cavalry,
    Siege,
    UnitMin = Peasent,
    UnitMax = Siege
};

enum ArmyType
{
    Attacker,
    Defender,
    ArmyTypeMin = Attacker,
    ArmyTypeMax = Defender
};

enum Result
{
    AttackerWins,
    DefenderWins,
    Draw
};

inline std::string asString(Unit unit)
{
    switch (unit) {
        case Peasent:
            return "peasent";
        case Archer:
            return "archer";
        case Infantry:
            return "infantry";
        case Cavalry:
            return "cavalry";
        case Siege:
            return "siege";
    }
}

inline std::string asString(ArmyType army)
{
    switch (army) {
        case Attacker:
            return "attacker";
        case Defender:
            return "defender";
    }
}

class UnitDefinition final
{
  public:
    UnitDefinition(Unit _unit,
                  unsigned int _hitsOn,
                  std::set<Unit>&& _counters)
        : unit(_unit)
        , hitsOn(_hitsOn)
        , counters(std::move(_counters))
    {
    }

    UnitDefinition() = delete;
    ~UnitDefinition() = default;
    UnitDefinition(const UnitDefinition&) = default;
    UnitDefinition& operator=(const UnitDefinition&) = delete;
    UnitDefinition(UnitDefinition&&) = delete;
    UnitDefinition& operator=(UnitDefinition&&) = delete;

    Unit getType()
    {
        return unit;
    }

    unsigned int doesHitOn()
    {
        return hitsOn;
    }

    bool isCounterOf(Unit _unit)
    {
        return _unit == Peasent or counters.find(_unit) != counters.end();
    }

  private:
    Unit           unit;
    unsigned int   hitsOn;
    std::set<Unit> counters;
};


class Army final
{
  public:
    Army(const std::string& _name,
         std::list<Unit>& _units)
        : name(_name)
        , units(_units)
    {
    }

    Army() = delete;
    ~Army() = default;
    Army(const Army&) = default;
    Army& operator=(const Army&) = delete;
    Army(Army&&) = delete;
    Army& operator=(Army&&) = delete;

    unsigned int countHits(UnitDefinition& unitRolling, Dice& dice)
    {
        unsigned int hits{0};
        unsigned int rolls = count(unitRolling.getType());

        if (rolls > 0) {
            LOG("\t\t\t" << name << " rolled ");

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

    unsigned int count(Unit unit)
    {
        return std::count(units.begin(), units.end(), unit);
    }

    unsigned int size()
    {
        return units.size();
    }

    void takeHits(UnitDefinition& attackingUnit, unsigned int hits)
    {
        // hits always target counters first
        for (auto unit = units.begin(); unit != units.end();) {
            if (hits == 0) {
                break;
            }

            if (attackingUnit.isCounterOf(*unit)) {
                LOG("\t\t\t" << name << " looses "
                    << asString(*unit) << " from counter "
                    << asString(attackingUnit.getType()) << std::endl);
                unit = units.erase(unit);
                --hits;
            } else {
                ++unit;
            }
        }

        // then they target other units based on ordering
        for (auto unit = units.begin(); unit != units.end();) {
            if (hits == 0) {
                break;
            }

            LOG("\t\t\t" << name << " looses "
                << asString(*unit) << " from non-counter "
                << asString(attackingUnit.getType()) << std::endl);

            unit = units.erase(unit);
            --hits;
        }

        if (hits != 0) {
            LOG("\t\t\t" << name << " " << hits << " unused hits" << std::endl);
        }
    }

    void takeHits(unsigned int hits = 1)
    {
        for (auto unit = units.begin(); unit != units.end();) {
            if (hits == 0) {
                break;
            }

            LOG("\t\t\t" << name << " looses "
                << asString(*unit) << " from general roll "
                << std::endl);

            unit = units.erase(unit);
            --hits;
        }

        if (hits != 0) {
            LOG("\t\t\t" << name << " " << hits << " unused hits" << std::endl);
        }
    }

    bool destroyed()
    {
        return units.empty();
    }

  private:
    std::string     name;
    std::list<Unit> units;
};


class Simulation final
{
  public:
    Simulation(std::vector<UnitDefinition>&& _unitDefinitions,
               std::list<Unit>&& _rankedAttackOrders,
               bool _generalRoll)
        : unitDefinitions(std::move(_unitDefinitions))
        , rankedAttackOrders(std::move(_rankedAttackOrders))
        , generalRoll(_generalRoll)
    {
    }

    Simulation() = delete;
    ~Simulation() = default;
    Simulation(const Simulation&) = delete;
    Simulation& operator=(const Simulation&) = delete;
    Simulation(Simulation&&) = delete;
    Simulation& operator=(Simulation&&) = delete;

    Result simulate(Army attacker, Army defender)
    {
        LOG(std::endl << "Simulation:" << std::endl);

        unsigned int roundNumber{1};
        do {
            LOG("\tRound " << roundNumber << std::endl);

            for (auto& rankedAttackOrder : rankedAttackOrders) {
                LOG("\t\tranked attack order "
                    << asString(rankedAttackOrder) << std::endl);

                auto& unitDef = unitDefinitions[rankedAttackOrder];
                auto offenseHits = attacker.countHits(unitDef, dice);
                auto defenseHits = defender.countHits(unitDef, dice);

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
        } while (not attacker.destroyed() and not defender.destroyed());

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

  private:
    void doGeneralRoll(Army& attacker, Army& defender)
    {
        auto attackerDice = std::min(3u, attacker.size());
        auto defenderDice = std::min(2u, defender.size());

        LOG("\t\tgeneral roll " << attackerDice << "a vs. "
            << defenderDice <<"d" << std::endl);

        auto attackerRolls = dice.roll(attackerDice);
        auto defenderRolls = dice.roll(defenderDice);

        while (not attackerRolls.empty() and not defenderRolls.empty()) {
            LOG("\t\t\tdice rolls were " << attackerRolls.top() << "a vs. "
                << defenderRolls.top() <<"d" << std::endl);

            if (attackerRolls.top() > defenderRolls.top()) {
                defender.takeHits();
            } else {
                attacker.takeHits();
            }

            attackerRolls.pop();
            defenderRolls.pop();
        }
    }

  private:
    Dice                        dice;
    std::vector<UnitDefinition> unitDefinitions;
    std::list<Unit>             rankedAttackOrders;
    bool                        generalRoll;
};


class ProgramOptions
    : public boost::program_options::options_description
{
  public:
    ProgramOptions()
        : boost::program_options::options_description("valid options")
    {
        add_options()
            ("num-simulations", boost::program_options::value<unsigned int>(
                &numSimulations)->default_value(100),
                "sets the number of simulations to run")
            ("general-roll", boost::program_options::value<bool>(
                &generalRoll)->default_value(false),
                "turns on the general roll")
            ("risk-europe", boost::program_options::value<bool>(
                &riskEurope)->default_value(false),
                "sets up the simulator to run for risk europe")
            ("verbose", boost::program_options::value<bool>(
                &verbose)->default_value(false),
                "turns on verbose loggging")
            ;

        for (int a{ArmyTypeMin}; a <= ArmyTypeMax; ++a) {
            ArmyType army = static_cast<ArmyType>(a);
            for (int u{UnitMin}; u <= UnitMax; ++u) {
                Unit unit = static_cast<Unit>(u);

                std::string option{asString(army) + "-" + asString(unit)};
                add_options()
                    (option.c_str(), boost::program_options::value<unsigned int>(
                        &armyUnitCounts[army][unit])->default_value(0),
                        "sets the specified unit count");
            }
        }
    }

    bool parseArgs(int argc, char** argv)
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

    void populateArmy(std::list<Unit>& units, ArmyType army)
    {
        for (int u{UnitMin}; u <= UnitMax; ++u) {
            Unit unit = static_cast<Unit>(u);
            for (auto c = 0; c < armyUnitCounts[army][unit]; ++c) {
                units.push_back(unit);
            }
        }
    }

    unsigned int numSimulations{1};
    bool         generalRoll{false};
    bool         riskEurope{false};

  private:
    unsigned int armyUnitCounts[ArmyTypeMax + 1][UnitMax + 1] = {};
};


// TODO siege need two dice.
#define RISK_EUROPE \
        { \
            {Peasent, 7, {}}, \
            {Archer, 5, {}}, \
            {Infantry, 7, {}}, \
            {Cavalry, 3, {}}, \
            {Siege, 3, {}} \
        }, \
        { \
            Siege, \
            Archer, \
            Cavalry, \
        }, true


#define SIMULATOR_ARGS \
        { \
            {Peasent, 6, {}}, \
            {Archer, 4, {Infantry}}, \
            {Infantry, 3, {Cavalry}}, \
            {Cavalry, 2, {Archer}}, \
            {Siege, 2, {Archer}} \
        }, \
        { \
            Siege, \
            Archer, \
            Cavalry, \
            Infantry, \
            Peasent \
        }


inline void runSimulations(Army& attacker,
                           Army& defender,
                           Simulation& simulation,
                           unsigned int numSimulations)
{
    unsigned int results[3] = {};
    for (unsigned int sim{0}; sim < numSimulations; ++sim) {
        ++results[simulation.simulate(attacker, defender)];
    }

    std::cout << "After " << numSimulations
        << " simulations:" << std::endl;
    std::cout << "\tthere were " << results[Draw] << " draws" << std::endl;
    std::cout << "\tthere were " << results[AttackerWins]
        << " attacker victories" << std::endl;
    std::cout << "\tthere were " << results[DefenderWins]
        << " defender victories" << std::endl;
}


int main(int argc, char** argv)
{
    ProgramOptions opts;
    if (opts.parseArgs(argc, argv) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    std::list<Unit> attackersUnits;
    std::list<Unit> defendersUnits;

    opts.populateArmy(attackersUnits, Attacker);
    opts.populateArmy(defendersUnits, Defender);

    Army attacker("Attacker", {attackersUnits});
    Army defender("Defender", {defendersUnits});

    if (opts.riskEurope) {
        Simulation simulation{RISK_EUROPE};
        runSimulations(attacker, defender, simulation, opts.numSimulations);
    } else {
        Simulation simulation{SIMULATOR_ARGS, opts.generalRoll};
        runSimulations(attacker, defender, simulation, opts.numSimulations);
    }

    return EXIT_SUCCESS;
}
