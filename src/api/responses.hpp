#pragma once

#include "response/choose_seat_response.hpp"
#include "response/choose_theater_response.hpp"
#include "response/welcome_response.hpp"

#include <variant>

namespace api
{
    using api_response = std::variant<
        response::choose_seat_response,
        response::choose_theater_response,
        response::welcome_response
    >;
}
