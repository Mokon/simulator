#include "Dice.hpp"

#include <algorithm>
#include <iostream>
#include <list>
#include <set>
#include <vector>

static constexpr bool printDetail{false};

enum Unit
{
  Peasent,
  Archer,
  Infanty,
  Cavalry
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
            return "Peasent";
        case Archer:
            return "Archer";
        case Infanty:
            return "Infanty";
        case Cavalry:
            return "Cavalry";
    }
}

class UnitDefintion
{
  public:
    UnitDefintion(Unit _unit,
                  unsigned int _hitsOn,
                  const std::set<Unit> _counters)
        : unit(_unit)
        , hitsOn(_hitsOn)
        , counters(_counters)
    {
    }

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
        return _unit == Unit::Peasent or counters.find(_unit) != counters.end();
    }

  private:
    Unit            unit;
    unsigned int    hitsOn;
    std::set<Unit> counters;
};


class Army
{
  public:
    Army(const std::string _name,
         const std::list<Unit>& _units)
        : name(_name)
        , units(_units)
    {
    }

    unsigned int countHits(UnitDefintion& unitDef, Dice& dice)
    {
        unsigned int hits{0};
        unsigned int rolls = count(unitDef.getType());

        if (rolls > 0) {
            if (printDetail) {
                std::cout << "\t\t\t" << name << " rolled ";
            }

            for (unsigned int roll{0} ; roll < rolls ; ++roll) {
                unsigned int value{dice.roll()};
            
                if (printDetail) {
                    std::cout << value << ( roll + 1 == rolls ? "": ",");
                }

                if (value >= unitDef.doesHitOn()) {
                    ++hits;
                }
            }
            
            if (printDetail) {
                std::cout << std::endl;
            }
        }
        return hits;
    }

    unsigned int count(Unit unit)
    {
        return std::count(units.begin(), units.end(), unit);
    }

    void takeHits(UnitDefintion& unitDef, unsigned int hits)
    {
        for (auto unit = units.begin(); unit != units.end();) {
            if (hits == 0) {
                break;
            }

            if (unitDef.isCounterOf(*unit)) {
                if (printDetail) {
                    std::cout << "\t\t\t" << name << " looses "
                        << asString(*unit) << " from counter" << std::endl;
                }
                unit = units.erase(unit);
                --hits;
            } else {
                ++unit;
            }
        }
        
        for (auto unit = units.begin(); unit != units.end();) {
            if (hits == 0) {
                break;
            }

            if (printDetail) {
                std::cout << "\t\t\t" << name << " looses "
                    << asString(*unit) << std::endl;
            }

            unit = units.erase(unit);
            --hits;
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


class Simulation
{
  public:
    Simulation(const std::vector<UnitDefintion>& _unitDefinitions,
               const std::list<Unit>& _rankedAttackOrders)
        : unitDefinitions(_unitDefinitions)
        , rankedAttackOrders(_rankedAttackOrders)
    {
    }

    Result simulate(Army attacker, Army defender)
    {
        if (printDetail) {
            std::cout << std::endl << "Starting Simulation" << std::endl;
        }

        bool retreat{false};

        unsigned int roundNumber{1};
        do {
            if (printDetail) {
                std::cout << "\tRound " << roundNumber << std::endl;
            }

            for (auto& rankedAttackOrder : rankedAttackOrders) {
                if (printDetail) {
                    std::cout << "\t\tranked attack order "
                        << asString(rankedAttackOrder) << std::endl;
                }
                auto& unitDef = unitDefinitions[rankedAttackOrder];
                auto offenseHits = attacker.countHits(unitDef, dice);
                auto defenseHits = defender.countHits(unitDef, dice);

                attacker.takeHits(unitDef, defenseHits);
                defender.takeHits(unitDef, offenseHits);

                if (attacker.destroyed() or defender.destroyed()) {
                    break;
                }
            }
            ++roundNumber;
        } while (not retreat and
                 not attacker.destroyed() and
                 not defender.destroyed());

        if (attacker.destroyed() and defender.destroyed()) {
            if (printDetail) {
                std::cout << "\tDraw, both armies destroyed" << std::endl;
            }
            return Draw;
        } else if (defender.destroyed()) {
            if (printDetail) {
                std::cout << "\tAttacker Wins" << std::endl;
            }
            return AttackerWins;
        } else {
            if (printDetail) {
                std::cout << "\tDefense Wins" << std::endl;
            }
            return DefenderWins;
        }
    }

  private:
    Dice dice;
    std::vector<UnitDefintion> unitDefinitions;
    std::list<Unit> rankedAttackOrders;
};


int main()
{
    Simulation simulation{
        {
            {Peasent, 6, {}},
            {Archer, 4, {Infanty}},
            {Cavalry, 2, {Archer}},
            {Infanty, 3, {Cavalry}}
        },
        {
            Unit::Archer,
            Unit::Cavalry,
            Unit::Infanty,
            Unit::Peasent
        }
    };

    Army attacker("Attacker", {Peasent,
                               Peasent,
                               Peasent,
                               Peasent,
                               Peasent,
                               Peasent,
                               Peasent,
                               Peasent,
                               Peasent,
                               Peasent,
                               Peasent,
                               Peasent,
                               Cavalry,
                               Cavalry,
                               Infanty,
                               Infanty,
                               Infanty,
                               Infanty,
                               Infanty,
                               Infanty
                              });
    Army defender("Defender", {Peasent,
                               Archer,
                               Archer,
                               Archer,
                               Cavalry,
                               Cavalry,
                               Cavalry,
                               Cavalry,
                               Cavalry,
                               Cavalry,
                               Cavalry,
                               Cavalry,
                               Cavalry,
                               Cavalry,
                               Cavalry,
                               Cavalry
                              });

    unsigned int numSimulations{100000};
    unsigned int results[3] = {};
    for (unsigned int sim{0}; sim < numSimulations; ++sim) {
        ++results[simulation.simulate(attacker, defender)];
    }
    
    std::cout << "After " << numSimulations << " simulations:" << std::endl;
    std::cout << "\there were " << results[Draw] << " draws" << std::endl;
    std::cout << "\there were " << results[AttackerWins] << " attacker victories" << std::endl;
    std::cout << "\there were " << results[DefenderWins] << " defender victories" << std::endl;

    return 0;
}
