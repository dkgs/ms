#pragma once

#include <string>

namespace service::model
{

    struct seat
    {
        enum class status {
            AVAILABLE, RESERVED
        };

        std::string id;
        status status;

        inline bool operator==(const seat & rhs) const
        {
            return id == rhs.id;
        }
    };

}