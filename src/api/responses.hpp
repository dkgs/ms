#pragma once

#include "response/choose_seat_response.hpp"
#include "response/choose_theater_response.hpp"
#include "response/welcome_response.hpp"
#include "response/health_response.hpp"

#include <variant>

namespace api
{
    /**
     * @brief All the possibles api responses
     */
    using api_response = std::variant<
        response::choose_seat_response,
        response::choose_theater_response,
        response::welcome_response,
        response::health_response
    >;
}
