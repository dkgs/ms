#pragma once

#include "model/movie.hpp"
#include "show_manager.hpp"

#include <vector>
#include <memory>
#include <optional>

namespace service 
{

    struct theater_manager
    {

        virtual ~theater_manager() {}

        /**
         * @brief Name of the theater that is managed
         * 
         * @return const std::string& 
         */
        virtual
        const std::string &
        get_name() const = 0;

        /**
         * @brief Add a new show manager
         * 
         * @param show_manager 
         * @return true if the show is well added, false if it already exists
         */
        virtual
        bool
        add_show_manager(std::shared_ptr<show_manager> show_manager) = 0;

        /**
         * @brief Returns all movies that are played by this theater
         * 
         * @return std::vector<model::movie> a list of all movies played
         */
        virtual
        std::vector<model::movie>
        fetch_movies() const = 0;

        /**
         * @brief queries the show manager for a specific movie
         * 
         * @param movie_name name of the movie
         * @return std::optional<std::shared_ptr<show_manager>> an optional pointer to the show manager if 
         */
        virtual
        std::optional<std::shared_ptr<show_manager>>
        get_show_manager(const std::string & movie_name) const = 0;

    };

}