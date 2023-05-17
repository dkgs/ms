#pragma once

#include "api/responses.hpp"

#include <boost/beast/http.hpp>
#include <sstream>

namespace api::serializer
{
    
    struct html_serializer
    {

        boost::beast::http::response<boost::beast::http::string_body>
        operator()(
            const boost::beast::http::request<boost::beast::http::string_body> & request,
            const response::choose_seat_response & response
        )
        {
            std::stringstream ss;
            html_header(ss, "Please choose a seat");
            if (response.message.has_value())
            {
                ss << "<div>" << response.message.value() << "</div>";
            }
            if (response.number_of_seats_selection.size() > 0)
            {
                ss << "<p>Book for a number of seats: ";
                for (const auto & num : response.number_of_seats_selection)
                {
                    ss << "<a href=\"/choose_seat/" << response.movie << "/" << response.theater << "/" << num << "\">" << num << "</a>  ";
                }
            }
            if (response.selected_number_of_seats == 1)
            {
                ss << "<p>Please choose a seat for the movie " << response.movie << " in the theater " << response.theater <<  ":</p>\n";
            }
            else
            {
                ss << "<p>Please choose a group of seat for the movie " << response.movie << " in the theater " << response.theater <<  ":</p>\n";
            }
            ss << "<ul>";
            for (const auto & seat_group : response.available_seat_groups)
            {
                ss << "<li><a href=\"/book_seat/" << response.movie << "/" << response.theater;
                for (const auto & seat : seat_group)
                {
                    ss << "/" << seat.id;
                }
                ss << "\">";
                for (const auto & seat : seat_group)
                {
                    ss << seat.id << " ";
                }
                ss << "</a></li>";
            }
            ss << "</ul>";
            html_footer(ss);

            return create_http_response(request, ss);
        }

        boost::beast::http::response<boost::beast::http::string_body>
        operator()(
            const boost::beast::http::request<boost::beast::http::string_body> & request,
            const response::choose_theater_response & response
        )
        {
            std::stringstream ss;
            html_header(ss, "Choose a theater");
            ss << "<p>Please choose a theater for the movie " << response.movie << ":</p>\n";
            ss << "<ul>";
            for (const auto & theater : response.theaters)
            {
                ss << "<li><a href=\"/choose_seat/" << response.movie << "/" << theater << "/1" << "\">" << theater << "</a></li>";
            }
            ss << "</ul>";
            html_footer(ss);

            return create_http_response(request, ss);
        }

        boost::beast::http::response<boost::beast::http::string_body>
        operator()(
            const boost::beast::http::request<boost::beast::http::string_body> & request,
            const response::welcome_response & response
        )
        {
            std::stringstream ss;
            html_header(ss, "Welcome!");
            if (response.message.has_value())
            {
                ss << "<div>" << response.message.value() << "</div>";
            }
            ss << "<p>Please choose a movie:</p>\n";
            ss << "<ul>";
            for (const auto & movie : response.playing_movies)
            {
                ss << "<li><a href=\"/choose_theater/" << movie.name << "\">" << movie.name << "</a><br />" << movie.synopsis << "</li>";
            }
            ss << "</ul>";
            html_footer(ss);

            return create_http_response(request, ss);
        }

        boost::beast::http::response<boost::beast::http::string_body>
        operator()(
            const boost::beast::http::request<boost::beast::http::string_body> & request,
            const response::health_response & response
        )
        {
            std::stringstream ss;
            ss << "{}";
            return create_http_response(request, ss);
        }

    private:

        void
        html_header(std::stringstream & ss, const std::string & title)
        {
            ss << "<html>\n";
            ss << "<head>\n";
            ss << "<title>" << title << "</title>";
            ss << "</head>\n";
            ss << "<body>\n";
        }

        void
        html_footer(std::stringstream & ss)
        {
            ss << "</body>\n";
            ss << "</html>\n";
        }

        boost::beast::http::response<boost::beast::http::string_body>
        create_http_response(
            const boost::beast::http::request<boost::beast::http::string_body> & request,
            std::stringstream & ss
        )
        {
            boost::beast::http::response<boost::beast::http::string_body> res {boost::beast::http::status::ok, request.version()};
            res.body() = ss.str();
            return res;
        }

    };

}
