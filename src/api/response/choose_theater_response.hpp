#pragma once

#include <vector>
#include <string>

namespace api::response
{
    struct choose_theater_response
    {
        std::string movie;
        std::vector<std::string> theaters;
    };
    
}
