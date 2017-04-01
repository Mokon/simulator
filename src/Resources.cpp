/* Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved */

#include "Resources.hpp"

Resources::Resources(const std::array<unsigned int, ResourceMax + 1>& _resources)
    : resources(_resources)
{
}

Resources
Resources::operator/(unsigned long rhs)
{
    Resources result{*this};
    for (int r{ResourceMin}; r <= ResourceMax; ++r) {
        Resource resource = static_cast<Resource>(r);
        result.resources[resource] /= rhs;
    }
    return result;
}

Resources&
Resources::operator+=(const Resources& rhs)
{
    for (int r{ResourceMin}; r <= ResourceMax; ++r) {
        Resource resource = static_cast<Resource>(r);
        resources[resource] += rhs.resources[resource];
    }
    return *this;
}

std::string
Resources::display() const
{
    std::stringstream out;
    
    bool first{true};
    for (int r{ResourceMin}; r <= ResourceMax; ++r) {
        Resource resource = static_cast<Resource>(r);

        if (resources[resource] != 0) {
            if (not first) {
                out << ", ";
            } else {
                first = false;
            }
        
            out << resources[resource] << " " << asString(resource);
        }
    }

    if (first) {
        out << "n/a";
    }

    return out.str();
}
