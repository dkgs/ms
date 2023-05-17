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
    struct root_handler
    {
        root_handler(std::shared_ptr<service::booking_service> booking_service) :
            booking_service_{std::move(booking_service)}
        {
        }

        bool
        can_handle(const std::vector<std::string> & route_split)
        {
            return route_split.size() == 1 && 
                (route_split.at(0).empty() || route_split.at(0) == "welcome");
        }

        api_response
        handle(const std::vector<std::string> & route_split, const boost::beast::http::request<boost::beast::http::string_body> & request)
        {
            auto movies = booking_service_->get_movies();
            return response::welcome_response{
                .playing_movies = movies
            };
        }

    private:

        std::shared_ptr<service::booking_service> booking_service_;

    };

}
