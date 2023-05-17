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
     * @brief Handles the endpoint /book of the api.
     * Books the selected seats and goes back to the welcome.
     * 
     */
    struct book_seat_handler
    {
        book_seat_handler(std::shared_ptr<service::booking_service> booking_service) :
            booking_service_{std::move(booking_service)}
        {
        }

        bool
        can_handle(const std::vector<std::string> & route_split)
        {
            return route_split.size() >= 5 && route_split.at(0) == "book_seat";
        }

        api_response
        handle(const std::vector<std::string> & route_split, const boost::beast::http::request<boost::beast::http::string_body> & request)
        {
            const auto & movie = route_split.at(1);
            const auto & theater = route_split.at(2);
            const auto & number_str = route_split.at(3);

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

            std::vector<std::string> seats {route_split.begin() + 3, route_split.end()};
            auto booking_result = show_manager_opt.value()->book_seats(seats);

            if (booking_result)
            {
                std::stringstream ss;
                ss << "Your seats ";
                for (const auto & s : seats)
                {
                    ss << s << " ";
                }
                ss << "have been booked successfully, enjoy your good movie ;)";

                return response::welcome_response{
                    .message = ss.str(),
                    .playing_movies = booking_service_->get_movies()
                };
            }
            else
            {
                // There is a bug here, did you find it ?
                unsigned int number = static_cast<unsigned int>(std::atoi(number_str.c_str()));
                auto seat_group_sizes = show_manager_opt.value()->authorized_seat_group_sizes();
                seat_group_sizes.erase(
                    std::remove(seat_group_sizes.begin(), seat_group_sizes.end(), number),
                    seat_group_sizes.end()
                );
                return response::choose_seat_response{
                    .message = "Booking failed, please choose new seats.",
                    .movie = movie,
                    .theater = theater,
                    .selected_number_of_seats = number,
                    .number_of_seats_selection = std::move(seat_group_sizes),
                    .available_seat_groups = show_manager_opt.value()->fetch_available_seats(1)
                };
            }
            
        }

    private:

        std::shared_ptr<service::booking_service> booking_service_;

    };

}
