#pragma once

#include "service/model/movie.hpp"

#include <vector>
#include <string>
#include <optional>

namespace api::response
{
    struct welcome_response
    {
        std::optional<std::string> message;
        std::vector<service::model::movie> playing_movies;
    };
    
}
