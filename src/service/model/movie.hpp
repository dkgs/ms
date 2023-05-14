#pragma once

#include <string>

namespace service::model
{

    struct movie
    {
        std::string name;
        std::string synopsis;

        inline bool operator==(const movie & rhs) const
        {
            return name == rhs.name;
        }
    };

}
