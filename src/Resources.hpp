/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <sstream>

enum Resource
    : uint8_t
{
    Gold,
    Food,
    Wood,
    Stone,
    Ore,
    ResourceMin = Gold,
    ResourceMax = Ore
};

inline std::string asString(Resource resource)
{
    switch (resource) {
        case Gold:
            return "gold";
        case Food:
            return "food";
        case Wood:
            return "wood";
        case Stone:
            return "stone";
        case Ore:
            return "ore";
        default:
            assert(false);
    }
}

class Resources final
{
  public:
    Resources(const std::array<unsigned int, ResourceMax + 1>& _resources);

    Resources() = default;
    ~Resources() = default;
    Resources(const Resources&) = default;
    Resources& operator=(const Resources&) = delete;
    Resources(Resources&&) = default;
    Resources& operator=(Resources&&) = default;
    
    Resources operator/(unsigned long rhs);
  
    Resources& operator+=(const Resources& rhs);

    std::string display() const;

  public:
    std::array<unsigned int, ResourceMax + 1> resources{};
};
