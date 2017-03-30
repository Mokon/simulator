/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

#include "Result.hpp"

#include <list>

class Results final
{
  public:
    Results() = default;
    ~Results() = default;
    Results(const Results&) = delete;
    Results& operator=(const Results&) = delete;
    Results(Results&&) = default;
    Results& operator=(Results&&) = delete;

    void add(Result&& result);

    long count(Winner winner) const;

    double meanRounds() const;

    double mean(Unit unit) const;

    double stdev(Unit unit, double& _mean) const;

    std::string summary(bool showStats) const;

  private:
    unsigned int sum(Unit unit) const;

  private:
    std::list<Result> results;
};
