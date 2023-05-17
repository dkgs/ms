#pragma once

#include "service/model/seat.hpp"

#include <vector>
#include <optional>

namespace api::response
{
    /**
     * @brief POD for a choose_seat request
     * 
     */
    struct choose_seat_response
    {
        std::optional<std::string> message;
        std::string movie;
        std::string theater;
        unsigned int selected_number_of_seats;
        std::vector<unsigned int> number_of_seats_selection;
        std::vector<std::vector<service::model::seat>> available_seat_groups;
    };
    
}
