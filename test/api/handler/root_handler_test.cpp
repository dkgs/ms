#include "api/handler/root_handler.hpp"

#include <catch2/catch.hpp>

struct booking_service_mock : public service::booking_service
{
    std::vector<service::model::movie>
    get_movies() const override
    {
        return {
            {.name="movie1", .synopsis=""},
            {.name="movie2", .synopsis=""}
        };
    }

    std::vector<std::string>
    fetch_theater_names_that_plays(const std::string & movie_name) const override
    {
        return {};
    }

    std::optional<std::shared_ptr<service::theater_manager>>
    get_theater_manager(const std::string & theater_name) const override
    {
        return {};
    }
};

SCENARIO("The root handler")
{
    api::handler::root_handler handler {
        std::make_unique<booking_service_mock>()
    };
    GIVEN("A / request ")
    {
        boost::beast::http::request<boost::beast::http::string_body> request {boost::beast::http::verb::get, "/", 10};
        std::vector<std::string> route_split {""};
        REQUIRE( handler.can_handle(route_split) );
        THEN("we expect a welcome response")
        {
            auto response = std::get<api::response::welcome_response>(handler.handle(route_split, request));
            REQUIRE( !response.message.has_value() );
            REQUIRE( response.playing_movies.at(0).name == "movie1" );
            REQUIRE( response.playing_movies.at(1).name == "movie2" );
        }
    }

    GIVEN("A /welcome request ")
    {
        boost::beast::http::request<boost::beast::http::string_body> request {boost::beast::http::verb::get, "/welcome", 10};
        std::vector<std::string> route_split {"welcome"};
        REQUIRE( handler.can_handle(route_split) );
        THEN("we expect a welcome response")
        {
            auto response = std::get<api::response::welcome_response>(handler.handle(route_split, request));
            REQUIRE( !response.message.has_value() );
            REQUIRE( response.playing_movies.at(0).name == "movie1" );
            REQUIRE( response.playing_movies.at(1).name == "movie2" );
        }
    }

    GIVEN("an unknown request")
    {
        THEN("It can't handle it")
        {
            REQUIRE( !handler.can_handle({"unkown"}) );
            REQUIRE( !handler.can_handle({"welcome", "bad"}) );
            REQUIRE( !handler.can_handle({"", "welcome"}) );
            REQUIRE( !handler.can_handle({}) );
        }
    }

}
