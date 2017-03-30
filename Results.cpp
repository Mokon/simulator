/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#include "Results.hpp"

#include <algorithm>
#include <sstream>

void
Results::add(Result&& result)
{
    results.emplace_back(std::move(result));
}

long
Results::count(Winner winner) const
{
    return std::count_if(results.begin(),
                         results.end(),
                         [&winner](const Result& result) {
                            return result.winner == winner;
                         });
}

double
Results::meanRounds() const
{
    return std::accumulate(results.begin(),
                           results.end(),
                           0.0,
                           [](double sum, const Result& result) {
                                return sum + result.numRounds;
                           })/static_cast<double>(results.size());
}

unsigned int
Results::sum(Unit unit) const
{
    return std::accumulate(results.begin(),
                           results.end(),
                           0u,
                           [unit](unsigned int sum, const Result& result) {
                                return sum + result.unitsRemaining[unit];
                           });
}

double
Results::mean(Unit unit) const
{
    return static_cast<double>(sum(unit))/static_cast<double>(results.size());
}

double
Results::stdev(Unit unit, double& _mean) const
{
    double accum = 0.0;
    std::for_each (results.begin(),
                   results.end(),
                   [&accum, &_mean, &unit](const Result& result) {
                       accum += std::pow(result.unitsRemaining[unit] - _mean, 2);
                   });

    return std::sqrt(accum /(results.size()-1));
}

std::string
Results::summary(bool showStats) const
{

    std::stringstream out;

    out << std::fixed;
    out.precision(3);

    out << "After " << results.size() << " simulations:" << std::endl;
    out << "\tthere were " << count(Draw) << " draws" << std::endl;
    out << "\tthere were " << count(AttackerWins)
        << " attacker victories" << std::endl;
    out << "\tthere were " << count(DefenderWins)
        << " defender victories" << std::endl;
    out << "\tthere were an average of " << meanRounds()
        << " rounds" << std::endl;

    if (showStats) {
        for (int u{UnitMin}; u <= UnitMax; ++u) {
            Unit unit = static_cast<Unit>(u);
            auto _mean = mean(unit);
            if (_mean != 0) {
                auto _stdev = stdev(unit, _mean);

                out << "\t\twith an average " << _mean << " " << asString(unit)
                    << " remaining, stdev " << _stdev << std::endl;
            }
        }
    }

    return out.str();
}
