#pragma once

#include <string>

namespace service::model
{

    /**
     * @brief POD for a movie description
     * 
     */
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
