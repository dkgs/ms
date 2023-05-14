#pragma once

#include "model/movie.hpp"
#include "model/seat.hpp"

#include <vector>
#include <memory>

namespace service 
{

    struct show_manager
    {       

        virtual ~show_manager() {}

        /**
         * @brief the movie in the show
         * 
         * @return const model::movie& 
         */
        virtual
        const model::movie &
        get_movie() const = 0;

        /**
         * @brief Returns a list of groups of seats that are available
         * 
         * @param number_of_contiguous_seats 
         * @return std::vector<std::vector<model::seat>> 
         */
        virtual
        std::vector<std::vector<model::seat>>
        fetch_available_seats(unsigned int number_of_contiguous_seats) const = 0;

        /**
         * @brief Books a set of seats
         * 
         * @param seats list of seats to be booked
         * @return true if all the seats have been booked
         * @return false if at least one of the seats are no more available
         */
        virtual
        bool
        book_seats(const std::vector<model::seat> & seats) = 0;

    };

}