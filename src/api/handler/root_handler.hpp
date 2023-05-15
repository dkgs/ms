#pragma once

#include "service/booking_service.hpp"

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

        boost::beast::http::response<boost::beast::http::string_body>
        handle(const std::vector<std::string> & route_split, const boost::beast::http::request<boost::beast::http::string_body> & request)
        {
            boost::beast::http::response<boost::beast::http::string_body> res {boost::beast::http::status::ok, request.version()};

            // TODO create a great body
            auto movies = booking_service_->get_movies();
            std::stringstream ss;
            for (const auto & movie : movies)
            {
                ss << movie.name << ": " << movie.synopsis << "\n";
            }
            res.body() = ss.str();
            return res;
        }

    private:

        std::shared_ptr<service::booking_service> booking_service_;

    };

}
