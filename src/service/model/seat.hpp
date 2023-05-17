#pragma once

#include <string>

namespace service::model
{
    /**
     * @brief POD for a seat
     * 
     */
    struct seat
    {
        /**
         * @brief Status of a seat
         * 
         */
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