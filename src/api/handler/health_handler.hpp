#pragma once

#include "api/responses.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <boost/beast/http.hpp>

namespace api::handler
{
    
    /**
     * @brief Handles the health endpoint of the api /health.
     * Permits to know if the service is still available
     * 
     */
    struct health_handler
    {
        health_handler()
        {}

        bool
        can_handle(const std::vector<std::string> & route_split)
        {
            return route_split.size() == 1 && 
                (route_split.at(0).empty() || route_split.at(0) == "health");
        }

        api_response
        handle(const std::vector<std::string> & route_split, const boost::beast::http::request<boost::beast::http::string_body> & request)
        {
            return response::health_response{};
        }

    };

}
