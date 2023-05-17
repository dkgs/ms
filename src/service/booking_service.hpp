#pragma once

#include "model/movie.hpp"
#include "theater_manager.hpp"

#include <memory>
#include <vector>
#include <optional>

namespace service 
{

    /**
     * @interface
     * @brief Point of entry to browse the different theatres and movies
     * 
     */
    struct booking_service
    {
        virtual ~booking_service() {};

        /**
         * @brief all movies that are played, independently of the theater
         * 
         * @return std::vector<model::movie> 
         */
        virtual
        std::vector<model::movie>
        get_movies() const = 0;

        /**
         * @brief queries the theaters that plays a specific movie
         * 
         * @param movie_name name of the movie
         * @return std::vector<std::string> the list of theater names that plays this movie
         */
        virtual
        std::vector<std::string>
        fetch_theater_names_that_plays(const std::string & movie_name) const = 0;

        /**
         * @brief queries the manager for a specific theater identified by its name
         * 
         * @param theater_name name of the theater
         * @return std::optional<theater_manager> the optional can be empty if no theater ghas been found
         */
        virtual
        std::optional<std::shared_ptr<theater_manager>>
        get_theater_manager(const std::string & theater_name) const = 0;

    };

}