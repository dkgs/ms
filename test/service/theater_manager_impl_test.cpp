#include "service/theater_manager_impl.hpp"

#include <catch2/catch.hpp>

using namespace service;

struct show_manager_mock : public show_manager
{
    const model::movie movie;

    show_manager_mock(const std::string & movie_name) :
        movie{.name=movie_name, .synopsis=""}
    {}

    const model::movie &
    get_movie() const override
    {
        return movie;
    }

    std::vector<std::vector<model::seat>>
    fetch_available_seats(unsigned int number_of_contiguous_seats) const override
    {
        return {};
    }

    bool
    book_seats(const std::vector<std::string> & seat_ids) override
    {
        return false;
    }
};

SCENARIO("classic usage of the theatre manager")
{
    GIVEN("a theatre manager with two shows")
    {
        theater_manager_impl tm {"Theatre1", {
            std::make_shared<show_manager_mock>("movie1"),
            std::make_shared<show_manager_mock>("movie2")
        }};

        WHEN("we fetch its name")
        {
            auto name = tm.get_name();
            THEN("we got it")
            {
                REQUIRE(name == "Theatre1");
            }
        }
        WHEN("we fetch the movies")
        {
            auto movies = tm.fetch_movies();

            THEN("we get all the movies")
            {
                REQUIRE( movies.size() == 2 );
                REQUIRE( std::find_if(movies.begin(), movies.end(), [](const auto & m) { return m.name == "movie1"; }) != movies.end() );
                REQUIRE( std::find_if(movies.begin(), movies.end(), [](const auto & m) { return m.name == "movie2"; }) != movies.end() );
            }
        }
        WHEN("we retrieve the show manager for an existing movie")
        {
            auto sm = tm.get_show_manager("movie1");
            THEN("we have it")
            {
                REQUIRE( sm.has_value() );
                REQUIRE( sm.value()->get_movie().name == "movie1" );
            }
        }
        WHEN("we retrieve the show manager for an unknown movie")
        {
            auto sm = tm.get_show_manager("unknown");
            THEN("we get nothing")
            {
                REQUIRE( !sm.has_value() );
            }
        }
    }

    GIVEN("an empty theatre")
    {
        theater_manager_impl tm {"Theatre1"};
        WHEN("we fetch the movies")
        {
            auto movies = tm.fetch_movies();

            THEN("there is none")
            {
                REQUIRE( movies.size() == 0 );
            }
        }
    }

}