#pragma once

#include "service/booking_service.hpp"
#include "api/responses.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <boost/beast/http.hpp>

namespace api::handler
{
    
    /**
     * @brief Handles the root endpoint of the api / or /welcome.
     * It returns all the movies that are showed
     * 
     */
    struct choose_theater_handler
    {
        choose_theater_handler(std::shared_ptr<service::booking_service> booking_service) :
            booking_service_{std::move(booking_service)}
        {
        }

        bool
        can_handle(const std::vector<std::string> & route_split)
        {
            return route_split.size() == 2 && route_split.at(0) == "choose_theater";
        }

        api_response
        handle(const std::vector<std::string> & route_split, const boost::beast::http::request<boost::beast::http::string_body> & request)
        {
            const auto & movie = route_split.at(1);
            
            return response::choose_theater_response{
                .movie = movie,
                .theaters = booking_service_->fetch_theater_names_that_plays(movie)
            };
        }

    private:

        std::shared_ptr<service::booking_service> booking_service_;

    };

}
