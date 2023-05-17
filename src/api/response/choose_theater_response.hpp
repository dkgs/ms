#pragma once

#include <vector>
#include <string>

namespace api::response
{
    /**
     * @brief POD for a choose_theater request
     * 
     */
    struct choose_theater_response
    {
        std::string movie;
        std::vector<std::string> theaters;
    };
    
}
