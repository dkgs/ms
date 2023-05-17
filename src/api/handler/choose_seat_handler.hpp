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
     * @brief Handles the endpoint /choose_seat
     * It returns the available seats to book.
     * 
     */
    struct choose_seat_handler
    {
        choose_seat_handler(std::shared_ptr<service::booking_service> booking_service) :
            booking_service_{std::move(booking_service)}
        {
        }

        bool
        can_handle(const std::vector<std::string> & route_split)
        {
            return route_split.size() == 4 && route_split.at(0) == "choose_seat";
        }

        api_response
        handle(const std::vector<std::string> & route_split, const boost::beast::http::request<boost::beast::http::string_body> & request)
        {
            const auto & movie = route_split.at(1);
            const auto & theater = route_split.at(2);
            unsigned int number = static_cast<unsigned int>(std::atoi(route_split.at(3).c_str()));

            auto theater_manager_opt = booking_service_->get_theater_manager(theater);
            if (!theater_manager_opt.has_value())
            {
                return response::welcome_response{
                    .message = "Bad theater name",
                    .playing_movies = booking_service_->get_movies()
                };
            }

            auto show_manager_opt = theater_manager_opt.value()->get_show_manager(movie);
            if (!show_manager_opt.has_value())
            {
                return response::welcome_response{
                    .message = "Bad movie name",
                    .playing_movies = booking_service_->get_movies()
                };
            }
            
            std::vector<unsigned int> seat_group_sizes = show_manager_opt.value()->authorized_seat_group_sizes();
            if (std::find(seat_group_sizes.begin(), seat_group_sizes.end(), number) == seat_group_sizes.end())
            {
                return response::choose_seat_response{
                    .message = "Invalid number of seats",
                    .movie = movie,
                    .theater = theater,
                    .selected_number_of_seats = seat_group_sizes.front(),
                    .number_of_seats_selection = {seat_group_sizes.begin() + 1, seat_group_sizes.end()},
                    .available_seat_groups = show_manager_opt.value()->fetch_available_seats(1)
                };
            }
            
            seat_group_sizes.erase(
                std::remove(seat_group_sizes.begin(), seat_group_sizes.end(), number),
                seat_group_sizes.end()
            );
            
            return response::choose_seat_response{
                .movie = movie,
                .theater = theater,
                .selected_number_of_seats = number,
                .number_of_seats_selection = seat_group_sizes,
                .available_seat_groups = show_manager_opt.value()->fetch_available_seats(number)
            };
        }

    private:

        std::shared_ptr<service::booking_service> booking_service_;

    };

}
